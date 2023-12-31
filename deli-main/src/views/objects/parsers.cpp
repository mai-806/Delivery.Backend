
#include "parsers.hpp"

#include <userver/logging/log.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace {
  enum class FieldType {
    kInt,
    kBool,
    kDouble,
    kString,
    kArray,
    kObject,
  };

  std::string ToString(FieldType type) {
    switch (type) {
      case FieldType::kInt:
        return "int";
      case FieldType::kBool:
        return "bool";
      case FieldType::kDouble:
        return "double";
      case FieldType::kString:
        return "string";
      case FieldType::kArray:
        return "array";
      case FieldType::kObject:
        return "object";
    }
  }

  FieldType GetFieldType(const userver::formats::json::Value &elem) {
    if (elem.IsBool()) {
      return FieldType::kBool;
    }
    if (elem.IsInt64()) {
      return FieldType::kInt;
    }
    if (elem.IsDouble()) {
      return FieldType::kDouble;
    }
    if (elem.IsString()) {
      return FieldType::kString;
    }
    if (elem.IsArray()) {
      return FieldType::kArray;
    }
    if (elem.IsObject()) {
      return FieldType::kObject;
    }
    throw std::runtime_error("Can't recognize type of object");
  }

  bool IsEqual(FieldType typel, FieldType typer) {
    if (typer != FieldType::kDouble)
      return typel == typer;
    const std::unordered_set<FieldType> d = {FieldType::kInt, FieldType::kDouble};
    return d.contains(typel);
  }


  using Types = std::unordered_map<std::string, FieldType>;
  using Keys = std::vector<std::string>;


  void CheckFields(const Keys &required_keys, const Keys &optional_keys, const Types &key_types,
                   const userver::formats::json::Value &elem) {
    for (const auto &key: required_keys) {
      if (!elem.HasMember(key)) {
        throw userver::formats::json::ParseException(
                fmt::format("Key '{}' is missing but required", key));
      }
      if (!IsEqual(GetFieldType(elem[key]), key_types.at(key))) {
        throw userver::formats::json::ParseException(
                fmt::format("Wrong type of key '{}', expected: '{}', got: '{}'",
                            key, ToString(key_types.at(key)), ToString(GetFieldType(elem[key]))));
      }
    }
    for (const auto &key: optional_keys) {
      if (elem.HasMember(key)) {
        if (!IsEqual(GetFieldType(elem[key]), key_types.at(key))) {
          throw userver::formats::json::ParseException(
                  fmt::format("Wrong type of key '{}', expected: '{}', got: '{}'",
                              key, ToString(GetFieldType(elem[key])), ToString(key_types.at(key))));
        }
      }
    }
  }

  template<typename T>
  T GetRequiredValue(const userver::formats::json::Value &elem, const std::string &key) {
    return elem[key].ConvertTo<T>();
  }

  template<typename T>
  std::optional<T> GetOptionalValue(const userver::formats::json::Value &elem, const std::string &key) {
    if (!elem.HasMember(key)) {
      return std::nullopt;
    }
    return std::optional(elem[key].ConvertTo<T>());
  }

  using Coordinate = deli_main::views::Coordinate;
  using OrderCreationRequest = deli_main::views::v1::order::post::OrderCreationRequest;
  using OrderCreationResponse = deli_main::views::v1::order::post::OrderCreationResponse;
  using OrderDto = deli_main::views::OrderDto;
  using OrderStatus = deli_main::views::OrderStatus;
  using OrderGetRequest = deli_main::views::v1::order::get::OrderGetRequest;
}

namespace userver::formats::parse {

  Coordinate Parse(const userver::formats::json::Value &elem,
                   userver::formats::parse::To<Coordinate>) {
    const Keys required_keys = {"lon", "lat"};
    const Keys optional_keys;
    const Types key_types = {
            {"lon", FieldType::kDouble},
            {"lat", FieldType::kDouble}
    };

    CheckFields(required_keys, optional_keys, key_types, elem);

    // required fields:
    Coordinate coordinate{
            .lon = GetRequiredValue<double>(elem, "lon"),
            .lat = GetRequiredValue<double>(elem, "lat")
    };
    if (coordinate.lat > 90 || coordinate.lat < -90) {
      throw userver::formats::json::ParseException("lat param out of bounds");
    }
    if (coordinate.lon > 180 || coordinate.lon < -180) {
      throw userver::formats::json::ParseException("lon param out of bounds");
    }

    return coordinate;
  }

  OrderCreationRequest
  Parse(const userver::formats::json::Value &elem,
        userver::formats::parse::To<OrderCreationRequest>) {
    const Keys required_keys = {"customerId", "start", "finish"};
    const Keys optional_keys;
    const Types key_types = {
            {"customerId", FieldType::kInt},
            {"start",      FieldType::kObject},
            {"finish",     FieldType::kObject}
    };

    CheckFields(required_keys, optional_keys, key_types, elem);
    LOG_DEBUG() << "fields are checked";
    // required fields:
    OrderCreationRequest order_creation_request{
            .customer_id = GetRequiredValue<int64_t>(elem, "customerId"),
            .start = GetRequiredValue<Coordinate>(elem, "start"),
            .finish = GetRequiredValue<Coordinate>(elem, "finish")
    };
    LOG_DEBUG() << "request parsed";
    return order_creation_request;
  }

  OrderGetRequest
  Parse(const userver::formats::json::Value &elem,
        userver::formats::parse::To<deli_main::views::v1::order::get::OrderGetRequest>) {
    const Keys required_keys = {"order_id"};
    const Keys optional_keys;
    const Types key_types = {
            {"order_id", FieldType::kInt}
    };
    CheckFields(required_keys, optional_keys, key_types, elem);
    LOG_DEBUG() << "fields are checked";
    OrderGetRequest order_get_request{
            .order_id = GetRequiredValue<int64_t>(elem, "order_id")
    };

    if (order_get_request.order_id < 0) {
      throw userver::formats::json::ParseException("id param should be above 0");
    }
    LOG_DEBUG() << "request parsed";

    return order_get_request;
  }
} // namespace userver::formats::parse


namespace userver::formats::serialize {

  json::Value Serialize(const deli_main::views::ErrorResponse &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["message"] = value.message;

    return builder.ExtractValue();
  }

  json::Value Serialize(const OrderCreationResponse &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["order_id"] = value.order_id;

    return builder.ExtractValue();
  }

  json::Value Serialize(const Coordinate &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["lon"] = value.lon;
    builder["lat"] = value.lat;

    return builder.ExtractValue();
  }

  json::Value Serialize(const OrderDto &value, serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["order_id"] = value.order_id;
    builder["customer_id"] = value.customer_id;
    builder["start_point"] = value.start_point;
    builder["end_point"] = value.end_point;
    builder["courier_id"] = value.courier_id;

    switch (value.status) {
      case OrderStatus::kOrderStatusNew:
        builder["status"] = "new";
        break;
      case OrderStatus::kOrderStatusWaiting:
        builder["status"] = "waiting";
        break;
      case OrderStatus::kOrderStatusInProgress:
        builder["status"] = "in_progress";
        break;
      case OrderStatus::kOrderStatusDelivered:
        builder["status"] = "delivered";
        break;
      case OrderStatus::kOrderStatusCanceled:
        builder["status"] = "canceled";
    }

    return builder.ExtractValue();
  }

  json::Value Serialize(const deli_main::views::v1::orders::get::GetOrdersResponse &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["orders"] = Serialize<std::vector<OrderDto>, json::Value>(value.orders, serialize::To<json::Value>());

    return builder.ExtractValue();
  }

  json::Value Serialize(const deli_main::views::v1::couriers::available::get::GetAvailableCouriersResponse &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["courier_ids"] = Serialize<std::vector<int64_t>, json::Value>(value.courier_ids,
                                                                          serialize::To<json::Value>());

    return builder.ExtractValue();
  }
} // namespace userver::formats::serialize
