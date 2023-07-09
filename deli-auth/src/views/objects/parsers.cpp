
#include "parsers.hpp"

#include <vector>

#include <userver/logging/log.hpp>

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

  using UserType = deli_auth::views::UserType;
  using ErrorResponse = deli_auth::views::ErrorResponse;
  using LogoutRequest = deli_auth::views::v1::auth::logout::post::LogoutRequest;
  using LogoutResponse200 = deli_auth::views::v1::auth::logout::post::LogoutResponse200;
}

namespace userver::formats::parse {

  ErrorResponse Parse(const userver::formats::json::Value &elem,
                      userver::formats::parse::To<ErrorResponse>) {
    const Keys required_keys = {"message"};
    const Keys optional_keys;
    const Types key_types = {
        {"message", FieldType::kString}
    };

    CheckFields(required_keys, optional_keys, key_types, elem);
    LOG_DEBUG() << "fields are checked";

    ErrorResponse error_response{
        .message = GetRequiredValue<std::string>(elem, "message")
    };
    LOG_DEBUG() << "request parsed";
    return error_response;
  }

  LogoutRequest Parse(const userver::formats::json::Value &elem,
                    userver::formats::parse::To<LogoutRequest>) {
    const Keys required_keys = {"id"};
    const Keys optional_keys;
    const Types key_types = {
        {"id", FieldType::kInt}
    };

    CheckFields(required_keys, optional_keys, key_types, elem);
    LOG_DEBUG() << "fields are checked";

    LogoutRequest logout_request{
        .id = GetRequiredValue<int64_t>(elem, "id")
    };

    LOG_DEBUG() << "request parsed";
    return logout_request;
  }

  LogoutResponse200 Parse(const userver::formats::json::Value &elem,
                        userver::formats::parse::To<LogoutResponse200>) {
    const Keys required_keys = {"id", "login", "is_auth"};
    const Keys optional_keys;
    const Types key_types = {
        {"id", FieldType:: kInt},
        {"login", FieldType:: kString},
        {"is_auth", FieldType:: kBool}
    };

    CheckFields(required_keys, optional_keys, key_types, elem);
    LOG_DEBUG() << "fields are checked";

    LogoutResponse200 logout_response_200{
        .id = GetRequiredValue<int64_t>(elem, "id"),
        .login = GetRequiredValue<std::string>(elem, "login"),
        .is_auth = GetRequiredValue<bool>(elem, "is_auth")
    };
    if (logout_response_200.is_auth){
      throw userver::formats::json::ParseException("is_auth must be false");
    }
    LOG_DEBUG() << "request parsed";
    return logout_response_200;
  }
}  // namespace userver::formats::parse


namespace userver::formats::serialize {

  json::Value Serialize(const ErrorResponse &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["message"] = value.message;

    return builder.ExtractValue();
  }

  json::Value Serialize(const LogoutRequest &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["id"] = value.id;

    return builder.ExtractValue();
  }

  json::Value Serialize(const LogoutResponse200 &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["id"] = value.id;
    builder["login"] = value.login;
    builder["is_auth"] = value.is_auth;

    return builder.ExtractValue();
  }
}