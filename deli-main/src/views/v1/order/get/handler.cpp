#include "handler.hpp"

#include <models/models.hpp>
#include <models/requests.hpp>

namespace {
  using OrderDto = deli_main::views::OrderDto;
  using ModelsOrderStatus = deli_main::models::OrderStatus;
  using ViewsOrderStatus = deli_main::views::OrderStatus;
  using ModelsCoordinate = deli_main::models::Coordinate;
  using ViewsCoordinate = deli_main::views::Coordinate;

  ViewsCoordinate ParseCoordinate(const ModelsCoordinate &coordinate) {
    return ViewsCoordinate{
            .lon = coordinate.longitude,
            .lat = coordinate.latitude
    };
  }

  ViewsOrderStatus ParseOrderStatus(const ModelsOrderStatus &status) {
    switch (status) {
      case ModelsOrderStatus::kOrderStatusNew:
        return ViewsOrderStatus::kOrderStatusNew;
      case ModelsOrderStatus::kOrderStatusCanceled:
        return ViewsOrderStatus::kOrderStatusCanceled;
      case ModelsOrderStatus::kOrderStatusDelivered:
        return ViewsOrderStatus::kOrderStatusDelivered;
      case ModelsOrderStatus::kOrderStatusInProgress:
        return ViewsOrderStatus::kOrderStatusInProgress;
      case ModelsOrderStatus::kOrderStatusWaiting:
        return ViewsOrderStatus::kOrderStatusWaiting;
    }
  }

  OrderDto ParseOrder(const deli_main::models::Order &order) {
    OrderDto res{};
    res.order_id = order.id;
    res.courier_id = order.courier;
    res.customer_id = order.customer;
    res.status = ParseOrderStatus(std::move(order.status));
    res.start_point = ParseCoordinate(std::move(order.start_point));
    res.end_point = ParseCoordinate(std::move(order.end_point));
    return res;
  }
}

namespace deli_main::views::v1::order::get {

  Handler::Handler(const userver::components::ComponentConfig &config,
                   const userver::components::ComponentContext &component_context) :
          userver::server::handlers::HttpHandlerJsonBase(config, component_context),
          requester_(component_context.FindComponent<components::Requester>()) {}


  userver::formats::json::Value Handler::HandleRequestJsonThrow(
          const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
          userver::server::request::RequestContext &) const try {

    const auto order_id = json.As<Request>().order_id;

    const auto order = requester_.DoDBQuery(models::requests::GetOrder, order_id);

    Response200 response200 = ParseOrder(order);

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
  } catch (const userver::storages::postgres::RuntimeError &exception) {
    request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
    return Serialize(
            Response404{
                    .message = exception.what()
            },
            userver::formats::serialize::To<userver::formats::json::Value>());
  }
} // namespace deli_main::views::v1::order::post
