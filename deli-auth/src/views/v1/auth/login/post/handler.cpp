#include <time.h>
#include <userver/crypto/base64.hpp>
#include <userver/crypto/hash.hpp>

#include "handler.hpp"

#include <common/exceptions.hpp>

#include <models/models.hpp>
#include <models/requests.hpp>

namespace {
  using ParseArgException = deli_auth::common::exceptions::ParseArgException;
}

namespace deli_auth::views::v1::auth::login::post {

  std::string GenerationToken(int64_t user_id, int64_t expires_in){

    const std::string header = "{\"alg\":\"HS256\",\"typ\":\"JWT\"}";
    const std::string payload = "{\"user_id\": "
                                + std::to_string(user_id)
                                + ", \"exp\":"
                                + std::to_string(expires_in)
                                + "}";

    const auto headerBase64 = userver::crypto::base64::Base64UrlEncode(header);
    const auto payloadBase64 = userver::crypto::base64::Base64UrlEncode(payload);

    const auto data = headerBase64 + '.' + payloadBase64;

    const auto secret = "123456";
    const auto sig = userver::crypto::hash::Sha256(data);

    const auto jwt = data + '.' + sig;

    return jwt;
  }


  Handler::Handler(const userver::components::ComponentConfig &config,
                   const userver::components::ComponentContext &component_context) :
    userver::server::handlers::HttpHandlerJsonBase(config, component_context),
    requester_(component_context.FindComponent<components::Requester>()) {}

  userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
    userver::server::request::RequestContext &) const try {

    const auto request_data = json.As<Request>();

    // Todo: придумать проверку на наличие поля login
    models::User user{
      .login = request_data.login
    };

    if (!request.HasHeader("password")) {
      throw ParseArgException("Wrong request: request must contain password field");
    }

    const auto& password_header = request.GetHeader("password");

    // Todo: понять, что возвращает DoDBQuery, если введенного логина не существует
    // возможно userver::storages::postgres::NonSingleRowResultSet

    try {
      const auto password_bd = requester_.DoDBQuery(models::requests::SelectPassword, user);
      if (password_header != password_bd){
        throw ParseArgException("Password is wrong");
      }
    } catch (...) {
      request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
      return Serialize(
          Response404{
              .message = "User not found"
          },
          userver::formats::serialize::To<userver::formats::json::Value>());
    }

    std::time_t time_now = std::time(nullptr);
    int64_t expires_in = static_cast<int64_t> (time_now) + 600;  // токен на 10 минут

    const auto user_id = requester_.DoDBQuery(models::requests::SelectUserID, user);
    std::string access_token = GenerationToken(user_id, expires_in);

    std::string refresh_token;
    for(int i = 0; i < 10; ++i) refresh_token = refresh_token + char('A'+ rand()%26);

    models::BearerTokens bearer_token {
      .user_id = user_id,
      .access_token = access_token,
      .refresh_token = refresh_token,
      .expires_in = expires_in
    };

    const auto bearer_token_id = requester_.DoDBQuery(models::requests::InsertToken, bearer_token);

    Response200 response200{
      .is_auth = true
    };

    request.SetResponseStatus(userver::server::http::HttpStatus::kOk);
    // Todo: вернуть токен через хедер

    return Serialize(
      response200,
      userver::formats::serialize::To<userver::formats::json::Value>());

  } catch (const ParseArgException &exception) {
    request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
    return Serialize(
      Response400{
        .message = exception.what()
      },
      userver::formats::serialize::To<userver::formats::json::Value>());
  }


} // namespace deli_auth::views::v1::auth::login::post
