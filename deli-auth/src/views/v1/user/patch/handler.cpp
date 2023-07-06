#include "handler.hpp"

#include <models/models.hpp>
#include <models/requests.hpp>

namespace deli_auth::views::v1::user::patch {

  Handler::Handler(const userver::components::ComponentConfig &config,
                   const userver::components::ComponentContext &component_context) :
          userver::server::handlers::HttpHandlerJsonBase(config, component_context),
          requester_(component_context.FindComponent<components::Requester>()) {}


  userver::formats::json::Value Handler::HandleRequestJsonThrow(
          const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
          userver::server::request::RequestContext &) const try {


    const auto request_data = json.As<Request>();

    if (request_data.login.has_value()) {

        // Update user login
        requester_.DoDBQuery(models::requests::UpdateUserLogin, request_data.id, request_data.login.value());

    } else if (request_data.userType.has_value()) {

        // Update user type
        requester_.DoDBQuery(models::requests::UpdateUserType, request_data.id, request_data.userType.value());

    }

    // Get updated user data from database
    const auto user = requester_.DoDBQuery(models::requests::GetUserById, request_data.id);

    Response200 response200 {
            .id = user.id,
            .login = user.login,
            .userType = user.user_type
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

} // namespace deli_auth::views::v1::user::patch