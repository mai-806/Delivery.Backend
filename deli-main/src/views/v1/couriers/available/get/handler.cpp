#include "handler.hpp"

#include <common/exceptions.hpp>

#include <models/models.hpp>
#include <models/requests.hpp>

#include <views/objects/objects.hpp>

namespace {
using ParseArgException = deli_main::common::exceptions::ParseArgException;

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

namespace deli_main::views::v1::couriers::available::get {

Handler::Handler(const userver::components::ComponentConfig &config,
                 const userver::components::ComponentContext &component_context) :
    userver::server::handlers::HttpHandlerJsonBase(config, component_context),
    requester_(component_context.FindComponent<components::Requester>()) {}


userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
    userver::server::request::RequestContext &) const try {

  const auto request_limit = ParseArg(request, "limit", 10);
  const auto request_offset = ParseArg(request, "offset", 0);

  const auto free_courier_ids = requester_.DoDBQuery(models::requests::GetFreeCourierIds, request_limit, request_offset);

  Response200 response200{
      .courier_ids = free_courier_ids
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

} // namespace deli_main::views::v1::couriers::available::get
