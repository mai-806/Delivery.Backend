
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
  using AuthRequest = deli_auth::views::v1::auth::login::post::AuthRequest;
  using AuthResponse200 = deli_auth::views::v1::auth::login::post::AuthResponse200;
}

namespace userver::formats::parse {
  // ХУЙ ЗНАЕТ КАК ПАРСИТЬ ENUM БЛЯТЬ ПОШЕЛ НАХУЙ
//  UserType Parse(const userver::formats::json::Value &elem,
//                 userver::formats::parse::To<UserType>) {
//    UserType user_type{
//      .customer = "customer",
//      .courier = "courier",
//      .admin = "admin"
//    };
//
//    return user_type;
//  }

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

  AuthRequest Parse(const userver::formats::json::Value &elem,
                    userver::formats::parse::To<AuthRequest>) {
    const Keys required_keys = {"login"};
    const Keys optional_keys;
    const Types key_types = {
      {"login", FieldType::kString}
    };

    CheckFields(required_keys, optional_keys, key_types, elem);
    LOG_DEBUG() << "fields are checked";

    AuthRequest auth_request{
      .login = GetRequiredValue<std::string>(elem, "login")
    };
    if (auth_request.login.empty()){
      throw userver::formats::json::ParseException("login is empty");
    }
    LOG_DEBUG() << "request parsed";
    return auth_request;
  }

  AuthResponse200 Parse(const userver::formats::json::Value &elem,
                        userver::formats::parse::To<AuthResponse200>) {
    const Keys required_keys = {"is_auth"};
    const Keys optional_keys;
    const Types key_types = {
      {"is_auth", FieldType:: kBool}
    };

    CheckFields(required_keys, optional_keys, key_types, elem);
    LOG_DEBUG() << "fields are checked";

    AuthResponse200 auth_response_200{
      .is_auth = GetRequiredValue<bool>(elem, "is_auth")
    };
    if (!auth_response_200.is_auth){
      throw userver::formats::json::ParseException("is_auth must be true");
    }
    LOG_DEBUG() << "request parsed";
    return auth_response_200;
  }
}  // namespace userver::formats::parse


namespace userver::formats::serialize {

  json::Value Serialize(const ErrorResponse &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["message"] = value.message;

    return builder.ExtractValue();
  }

  json::Value Serialize(const AuthRequest &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["login"] = value.login;

    return builder.ExtractValue();
  }

  json::Value Serialize(const AuthResponse200 &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["is_auth"] = value.is_auth;

    return builder.ExtractValue();
  }
}
