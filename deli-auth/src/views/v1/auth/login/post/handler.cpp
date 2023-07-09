#include "handler.hpp"

#include <common/exceptions.hpp>

#include <models/models.hpp>
#include <models/requests.hpp>

namespace {
  using ParseArgException = deli_auth::common::exceptions::ParseArgException;
}

namespace deli_auth::views::v1::auth::login::post {

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
    } catch (...) { // я знаю, что это плохая практика, но предложите что-нибудь получше тогда
      request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
      return Serialize(
          Response404{
              .message = "User not found"
          },
          userver::formats::serialize::To<userver::formats::json::Value>());
    }

    // если все ок, генерируем токен, добавляем в бд и возвращаем в хедере

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
