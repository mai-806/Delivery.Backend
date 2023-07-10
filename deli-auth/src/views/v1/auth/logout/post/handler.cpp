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

namespace deli_auth::views::v1::auth::logout::post {


  Handler::Handler(const userver::components::ComponentConfig &config,
                   const userver::components::ComponentContext &component_context) :
      userver::server::handlers::HttpHandlerJsonBase(config, component_context),
      requester_(component_context.FindComponent<components::Requester>()) {}

  userver::formats::json::Value Handler::HandleRequestJsonThrow(
      const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
      userver::server::request::RequestContext &) const try {

    const auto request_data = json.As<Request>();

    models::User user{
        .id = request_data.id
    };
    std::string login = "";
    try {
      login = requester_.DoDBQuery(models::requests::SelectLogin, user);
    } catch (...) {
      request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
      return Serialize(
          Response404{
              .message = "User not found"
          },
          userver::formats::serialize::To<userver::formats::json::Value>());
    }

    try {
      requester_.DoDBQuery(models::requests::SelectBearerTokens, user);
    } catch (...) {
      request.SetResponseStatus(userver::server::http::HttpStatus::kUnauthorized);
      return Serialize(
          Response401{
              .message = "Unauthorized user"
          },
          userver::formats::serialize::To<userver::formats::json::Value>());
    }

    requester_.DoDBQuery(models::requests::DeleteAccessTokenWhereUserID, user);

    Response200 response200{
        .id = request_data.id,
        .login = login,
        .is_auth = false
    };

    request.SetResponseStatus(userver::server::http::HttpStatus::kOk);

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


} // namespace deli_auth::views::v1::auth::logout::post
