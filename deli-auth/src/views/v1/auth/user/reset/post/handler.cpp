#include "handler.hpp"

#include <models/models.hpp>
#include <models/requests.hpp>

namespace deli_auth::views::v1::auth::user::reset::post {

    Handler::Handler(const userver::components::ComponentConfig &config,
                     const userver::components::ComponentContext &component_context) :
            userver::server::handlers::HttpHandlerJsonBase(config, component_context),
            requester_(component_context.FindComponent<components::Requester>()) {}


    userver::formats::json::Value Handler::HandleRequestJsonThrow(
            const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
            userver::server::request::RequestContext &) const try {

        const auto request_data = json.As<Request>();
        const auto new_password = request.GetHeader("password");

        models::User user {
                .id = request_data.id,
                .password = new_password
        };

        const auto result = requester_.DoDBQuery(models::requests::UpdateUserPassword, user);


        if (result == request_data.id) {
            request.SetResponseStatus(userver::server::http::HttpStatus::kNoContent);

            return userver::formats::json::FromString(R"({})");

        } else if(result == -1) {
            request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
            return Serialize(
                    Response404{
                            .message = "not found"
                    },
                    userver::formats::serialize::To<userver::formats::json::Value>());

        }
    } catch (const userver::formats::json::ParseException &exception) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return Serialize(
                Response400{
                        .message = "bad request"
                },
                userver::formats::serialize::To<userver::formats::json::Value>());
    }

} // namespace deli_auth::views::v1::auth::user::reset::post