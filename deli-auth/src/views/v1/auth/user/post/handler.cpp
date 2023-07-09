#include "handler.hpp"

#include <models/models.hpp>
#include <models/requests.hpp>

#include <userver/crypto/hash.hpp>

namespace {
  deli_auth::models::UserType viewsUTtoModels(deli_auth::views::UserType ut){
    switch (ut)
    {
    case deli_auth::views::UserType::kUserTypeCustomer:
      return deli_auth::models::UserType::kUserTypeCustomer;
    case deli_auth::views::UserType::kUserTypeCourier:
      return deli_auth::models::UserType::kUserTypeCourier;
    case deli_auth::views::UserType::kUserTypeAdmin:
      return deli_auth::models::UserType::kUserTypeAdmin;
    }
  }
}

namespace deli_auth::views::v1::auth::user::post {

  Handler::Handler(const userver::components::ComponentConfig &config,
                   const userver::components::ComponentContext &component_context) :
          userver::server::handlers::HttpHandlerJsonBase(config, component_context),
          requester_(component_context.FindComponent<components::Requester>()) {}


  userver::formats::json::Value Handler::HandleRequestJsonThrow(
          const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
          userver::server::request::RequestContext &) const try {

    const auto request_data = json.As<Request>();
    const auto header = request.GetHeader("password");

    if (header.empty()){
      request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return Serialize(
            Response400{
                    .message = "Key 'password' is missing but required"
            },
            userver::formats::serialize::To<userver::formats::json::Value>());
    }

    models::UserRegisterRequest user{
            .login = std::move(request_data.login),
            .password = std::move(userver::crypto::hash::Sha256(header.data())),
            .user_type = viewsUTtoModels(request_data.user_type)
    };

    const auto user_id = requester_.DoDBQuery(models::requests::InsertUser, user);

    if (user_id == -1){
      request.SetResponseStatus(userver::server::http::HttpStatus::kConflict);
      return Serialize(
            Response409{
                    .message = "login already exists"
            },
            userver::formats::serialize::To<userver::formats::json::Value>());
    }

    Response200 response200{
            .id = user_id
    };

    request.SetResponseStatus(userver::server::http::HttpStatus::kOk);

    return Serialize(
            response200,
            userver::formats::serialize::To<userver::formats::json::Value>());

  } catch (const userver::formats::json::ParseException &exception) {
    request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
    return Serialize(
            Response400{
                    .message = exception.what()
            },
            userver::formats::serialize::To<userver::formats::json::Value>());
  }

} // namespace deli_auth::views::v1::auth::user::post
