#include "handler.hpp"

#include <models/models.hpp>
#include <models/requests.hpp>

namespace deli_main::views::v1::order::post {

  Handler::Handler(const userver::components::ComponentConfig &config,
                   const userver::components::ComponentContext &component_context) :
          userver::server::handlers::HttpHandlerJsonBase(config, component_context),
          requester_(component_context.FindComponent<components::Requester>()) {}


  userver::formats::json::Value Handler::HandleRequestJsonThrow(
          const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
          userver::server::request::RequestContext &) const try {

    const auto request_data = json.As<Request>();

    models::User user {
            .id = request_data.id,
            .login = std::move(request_data.login),
            .user_type = std::move(request_data.userType)
    };

    const auto order_id = requester_.DoDBQuery(models::requests::InsertOrder, order);

    Response200 response200{
            .order_id = order_id
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

} // namespace deli_main::views::v1::order::post