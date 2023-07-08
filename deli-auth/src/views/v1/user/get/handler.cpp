#include "handler.hpp"

#include <models/models.hpp>
#include <models/requests.hpp>

namespace {

    using UserTypeViews = deli_auth::views::UserType;
    using UserTypeModels = deli_auth::models::UserType;

    UserTypeViews ParseUserType (const UserTypeModels type){
        switch(type){
            case UserTypeModels::kCustomer:
                return UserTypeViews::kCustomer;
            case UserTypeModels::kCourier:
                return UserTypeViews::kCourier;
            case UserTypeModels::kAdmin:
                return UserTypeViews::kAdmin;
        }
    }
}


namespace deli_auth::views::v1::user::get {

    Handler::Handler(const userver::components::ComponentConfig &config,
                     const userver::components::ComponentContext &component_context) :
            userver::server::handlers::HttpHandlerJsonBase(config, component_context),
            requester_(component_context.FindComponent<components::Requester>()) {}

    userver::formats::json::Value Handler::HandleRequestJsonThrow(
            const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
            userver::server::request::RequestContext &) const try {

        const auto request_data = json.As<Request>();

        models::User user{
            .id = request_data.id,
            .login = std::move(request_data.login)

        };

        const auto user_response = requester_.DoDBQuery(models::requests::UserGet, user);

        Response200 response200{
                .login = user_response.login,
                .user_type = ParseUserType(user_response.type),
        };

        request.SetResponseStatus(userver::server::http::HttpStatus::kOk);

        return Serialize(
                response200,
                userver::formats::serialize::To<userver::formats::json::Value>());

    }
    catch (const userver::formats::json::ParseException &exception) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return Serialize(
                Response400{
                        .message = exception.what()
                },
                userver::formats::serialize::To<userver::formats::json::Value>());
    }


} // namespace deli_main::views::v1::order::post
