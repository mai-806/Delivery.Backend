#include "handler.hpp"

#include <models/models.hpp>
#include <models/requests.hpp>

namespace deli_main::views::v1::order::change_status::post {

  Handler::Handler(const userver::components::ComponentConfig &config,
                   const userver::components::ComponentContext &component_context) :
          userver::server::handlers::HttpHandlerJsonBase(config, component_context),
          requester_(component_context.FindComponent<components::Requester>()) {}


  userver::formats::json::Value Handler::HandleRequestJsonThrow(
          const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
          userver::server::request::RequestContext &) const try {

    const auto request_data = json.As<Request>();

    models::Order order{
            .id = request_data.id,
            .status = request_data.status
    };

    const auto check = requester_.DoDBQuery(models::requests::UpdateOrderStatus, order);

    if (check == 1) {
      request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
      return Serialize(
              Response404{
                      .message = "Order not found"
              },
              userver::formats::serialize::To<userver::formats::json::Value>());
      }
    }

    request.SetResponseStatus(userver::server::http::HttpStatus::kNoContent);

    return Serialize(
            {},
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
