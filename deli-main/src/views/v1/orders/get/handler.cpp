#include "handler.hpp"

#include <models/models.hpp>
#include <models/requests.hpp>

#include <views/objects/objects.hpp>

namespace {
class ParseArgException : public std::exception {
 public:
  explicit ParseArgException(std::string msg) : msg_(std::move(msg)) {}

  const char* what() const noexcept final { return msg_.c_str(); }

 private:
  std::string msg_;
};

using ModelsOrderStatus = deli_main::models::OrderStatus;
using ViewsOrderStatus = deli_main::views::OrderStatus;
using ModelsCoordinate = deli_main::models::Coordinate;
using ViewsCoordinate = deli_main::views::Coordinate;
using OrderDto = deli_main::views::OrderDto;

ViewsOrderStatus ParseOrderStatus(const ModelsOrderStatus status) {
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

ViewsCoordinate ParseCoordinate(const ModelsCoordinate coordinate) {
  return ViewsCoordinate {
    .lon = coordinate.longitude,
    .lat = coordinate.latitude
  };
}

std::vector<OrderDto> ParseOrders(const std::vector<deli_main::models::Order>& v) {
  std::vector<OrderDto> res(v.size());
  for (size_t i=0; i < v.size(); ++i) {
    res[i].order_id = v[i].id;
    res[i].courier_id = v[i].courier;
    res[i].customer_id = v[i].customer;
    res[i].status = ParseOrderStatus(v[i].status);
    res[i].start_point = ParseCoordinate(v[i].start_point);
    res[i].end_point = ParseCoordinate(v[i].end_point);
  }
  return res;
}

uint64_t ParseArg(const userver::server::http::HttpRequest &request, const std::string& arg_name, uint64_t default_value) {
  if (!request.HasArg(arg_name)) {
    return default_value;
  }

  const std::string& arg = request.GetArg(arg_name);
  int64_t res;
  try {
     res = std::stoll(arg);
  } catch (std::invalid_argument &exception) {
    throw ParseArgException("Wrong request: error converting parameter '" + arg_name + "'");
  } catch (std::out_of_range &exception) {
    throw ParseArgException("Wrong request: parameter '" + arg_name + "' is too large");
  }

  if (res < 0) {
    throw ParseArgException("Wrong request: parameter '" + arg_name + "' must be greater than or equal to zero");
  }
  return res;
}

}

namespace deli_main::views::v1::orders::get {

Handler::Handler(const userver::components::ComponentConfig &config,
                 const userver::components::ComponentContext &component_context) :
    userver::server::handlers::HttpHandlerJsonBase(config, component_context),
    requester_(component_context.FindComponent<components::Requester>()) {}


userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
    userver::server::request::RequestContext &) const try {

  const auto request_limit = ParseArg(request, "limit", 10);
  const auto request_offset = ParseArg(request, "offset", 0);

  const auto orders = requester_.DoDBQuery(models::requests::GetOrders, request_limit, request_offset);

  Response200 response200{
      .orders = ParseOrders(orders)
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

} // namespace deli_main::views::v1::orders::get
