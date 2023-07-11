
#include "parsers.hpp"

#include <userver/logging/log.hpp>
#include <vector>

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

  using ErrorResponse = deli_auth::views::ErrorResponse;
  using AuthRequest = deli_auth::views::v1::auth::login::post::AuthRequest;
  using AuthResponse200 = deli_auth::views::v1::auth::login::post::AuthResponse200;
  using RegisterRequest = deli_auth::views::v1::auth::user::post::RegisterRequest;
  using RegisterResponse = deli_auth::views::v1::auth::user::post::RegisterResponse;
  using LogoutRequest = deli_auth::views::v1::auth::logout::post::LogoutRequest;
  using LogoutResponse200 = deli_auth::views::v1::auth::logout::post::LogoutResponse200;
  using UserType = deli_auth::views::UserType;
  using UserResetRequest = deli_auth::views::v1::auth::user::reset::post::UserResetRequest;
  using UserGetResponse = deli_auth::views::v1::user::get::UserGetResponse;
  using UserUpdateRequest = deli_auth::views::v1::user::patch::UserUpdateRequest;
  using UserUpdateResponse200 = deli_auth::views::v1::user::patch::UserUpdateResponse200;

}

namespace userver::formats::parse {
  
  UserUpdateRequest
  Parse(const userver::formats::json::Value &elem,
        userver::formats::parse::To<UserUpdateRequest>) {
    const Keys required_keys = {"id"};
    const Keys optional_keys = {"login", "user_type",};
    const Types key_types = {
            {"id", FieldType::kInt},
            {"login", FieldType::kString},
            {"user_type", FieldType::kString}
    };

    CheckFields(required_keys, optional_keys, key_types, elem);
    LOG_DEBUG() << "fields are checked";

    std::string user_type = GetRequiredValue<std::string>(elem, "user_type");
    deli_auth::views::UserType enum_user_type;
    if (user_type == "customer") {
        enum_user_type = deli_auth::views::UserType::kUserTypeCustomer;
    } else if (user_type == "courier") {
        enum_user_type = deli_auth::views::UserType::kUserTypeCourier;
    } else if (user_type == "admin") {
        enum_user_type = deli_auth::views::UserType::kUserTypeAdmin;
    } else {
        throw userver::formats::json::ParseException(
                fmt::format("user_type is invalid"));
    }

    UserUpdateRequest user_update_request {
            .id = GetRequiredValue<int64_t>(elem, "id"),
            .login = GetOptionalValue<std::string>(elem, "login"),
            .user_type = enum_user_type
    };
    LOG_DEBUG() << "request parsed";
    return user_update_request;
  }

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
      {"is_auth", FieldType:: kBool},
      {"access_token", FieldType:: kString}
    };

    CheckFields(required_keys, optional_keys, key_types, elem);
    LOG_DEBUG() << "fields are checked";

    AuthResponse200 auth_response_200{
      .is_auth = GetRequiredValue<bool>(elem, "is_auth"),
      .access_token = GetRequiredValue<std::string>(elem, "access_token")
    };
    if (!auth_response_200.is_auth){
      throw userver::formats::json::ParseException("is_auth must be true");
    }
    LOG_DEBUG() << "request parsed";
    return auth_response_200;
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

    RegisterRequest Parse(const userver::formats::json::Value &elem,
                          userver::formats::parse::To<RegisterRequest>) {
        const Keys required_keys = {"login", "user_type"};
        const Keys optional_keys;
        const Types key_types = {
                {"login",     FieldType::kString},
                {"user_type", FieldType::kString}
        };

        CheckFields(required_keys, optional_keys, key_types, elem);
        LOG_DEBUG() << "fields are checked";
        std::string user_type = GetRequiredValue<std::string>(elem, "user_type");
        UserType enum_user_type;
        if (user_type == "customer") {
            enum_user_type = UserType::kUserTypeCustomer;
        } else if (user_type == "courier") {
            enum_user_type = UserType::kUserTypeCourier;
        } else if (user_type == "admin") {
            enum_user_type = UserType::kUserTypeAdmin;
        } else {
            throw userver::formats::json::ParseException(
                    fmt::format("user_type is shit"));
        }
        // required fields:
        RegisterRequest register_request{
                .login = GetRequiredValue<std::string>(elem, "login"),
                .user_type = enum_user_type
        };
        LOG_DEBUG() << "request parsed";
        return register_request;
    }

    UserResetRequest
    Parse(const userver::formats::json::Value &elem,
          userver::formats::parse::To<UserResetRequest>) {
        const Keys required_keys = {"id"};
        const Keys optional_keys;
        const Types key_types = {
                {"id", FieldType::kInt}
        };

        CheckFields(required_keys, optional_keys, key_types, elem);
        LOG_DEBUG() << "fields are checked";
        // required fields:
        UserResetRequest user_reset_request{
                .id = GetRequiredValue<int64_t>(elem, "id"),
        };
        LOG_DEBUG() << "request parsed";
        return user_reset_request;
    }
} // namespace userver::formats::parse


namespace userver::formats::serialize {

  json::Value Serialize(const UserUpdateResponse200 &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["id"] = value.id;
    builder["login"] = value.login;
    builder["user_type"] = value.user_type;

    return builder.ExtractValue();
  }

    json::Value Serialize(const RegisterResponse &value,
                          serialize::To<json::Value>) {
        json::ValueBuilder builder;

        builder["id"] = value.id;

        return builder.ExtractValue();
    }
  
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
  
  json::Value Serialize(const LogoutRequest &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["id"] = value.id;
      
    return builder.ExtractValue();
  }

  json::Value Serialize(const AuthResponse200 &value,
                        serialize::To<json::Value>) {
    json::ValueBuilder builder;

    builder["is_auth"] = value.is_auth;
    builder["access_token"] = value.access_token;

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


    json::Value Serialize(const UserGetResponse &value,
                          serialize::To<json::Value>) {
        json::ValueBuilder builder;

        builder["login"] = value.login;
        switch (value.user_type) {
            case UserType::kUserTypeCustomer:
                builder["user_type"] = "customer";
                break;
            case UserType::kUserTypeCourier:
                builder["user_type"] = "courier";
                break;
            case UserType::kUserTypeAdmin:
                builder["user_type"] = "admin";
                break;
        }
        return builder.ExtractValue();
    }

} // namespace userver::formats::serialize
