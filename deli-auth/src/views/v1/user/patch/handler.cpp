#include "handler.hpp"

#include <models/models.hpp>
#include <models/requests.hpp>

namespace {

    using UserTypeModels = deli_auth::models::UserType;
    using UserTypeViews = deli_auth::views::UserType;

    std::string UserTypeToString(UserTypeModels user_type) {
        switch (user_type) {
            case UserTypeModels::kUserTypeCustomer:
                return "customer";
            case UserTypeModels::kUserTypeCourier:
                return "courier";
            case UserTypeModels::kUserTypeAdmin:
                return "admin";
            default:
                throw std::invalid_argument("Invalid user_type");
        }
    }

    UserTypeModels Parser(UserTypeViews user_type) {
        switch (user_type) {
            case UserTypeViews::kUserTypeCustomer:
                return UserTypeModels::kUserTypeCustomer;
            case UserTypeViews::kUserTypeCourier:
                return UserTypeModels::kUserTypeCourier;
            case UserTypeViews::kUserTypeAdmin:
                return UserTypeModels::kUserTypeAdmin;
        }
    }

} // namespace

namespace deli_auth::views::v1::user::patch {

  Handler::Handler(const userver::components::ComponentConfig &config,
                   const userver::components::ComponentContext &component_context) :
          userver::server::handlers::HttpHandlerJsonBase(config, component_context),
          requester_(component_context.FindComponent<components::Requester>()) {}


  userver::formats::json::Value Handler::HandleRequestJsonThrow(
          const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
          userver::server::request::RequestContext &) const try {


    const auto request_data = json.As<Request>();

    // Check if user exists
    const auto user_exists = requester_.DoDBQuery(models::requests::CheckUserExists, request_data.id);

    if (!user_exists) {
        // User not found
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return Serialize(
                Response404{
                    .message = "user not found"
                },
                userver::formats::serialize::To<userver::formats::json::Value>());
    }

    bool login_exist = request_data.login.has_value();
    bool user_type_exist = request_data.user_type.has_value();

    if (!login_exist && !user_type_exist) {

        throw userver::formats::json::ParseException("login and user_type are missing, but they are required");

    }

    if (login_exist) {

        // Update user login
        requester_.DoDBQuery(models::requests::UpdateUserLogin, request_data.id, request_data.login.value());

    }
    
    if (user_type_exist) {

        // Update user type
        requester_.DoDBQuery(models::requests::UpdateUserType, request_data.id, Parser(request_data.user_type.value()));

    }

    // Get updated user data from database
    const auto user = requester_.DoDBQuery(models::requests::GetUserById, request_data.id);

    Response200 response200 {
            .id = user.id,
            .login = user.login,
            .user_type = std::move(UserTypeToString(user.user_type))
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