#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/dynamic_config/source.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/clients/http/component.hpp>


namespace deli_main::clients::deli_auth {

  namespace v1_user_get {
    enum class UserType {
      kCustomer,
      kCourier,
      kAdmin,
    };
    struct Request {
      std::optional<std::string> login;
      std::optional<int64_t> id;
    };

    class ErrorResponse : public std::runtime_error {
      using runtime_error::runtime_error;
    };

    struct Response200 {
      std::string login;
      UserType user_type;
    };

    struct Response400 : public ErrorResponse {
      using ErrorResponse::ErrorResponse;
    };

    struct Response404 : public Response400 {
      using Response400::Response400;
    };

    inline std::string ToString(const UserType& type) {
      switch (type) {
        case UserType::kCustomer:
          return "customer";
        case UserType::kCourier:
          return "courier";
        case UserType::kAdmin:
          return "admin";
      }
    }
  }


  class Client : public userver::components::LoggableComponentBase {
  public:
    static constexpr auto kName = "deli-auth-client";

    Client(const userver::components::ComponentConfig &config,
           const userver::components::ComponentContext &context);

    v1_user_get::Response200 V1UserGet(const v1_user_get::Request &request);

    static userver::yaml_config::Schema GetStaticConfigSchema();
  private:
    userver::clients::http::Client &http_client_;
    const std::string v1_user_get_url_;
  };


} // namespace deli_main::clients::deli_auth

namespace {
  using V1UserGetUserType = deli_main::clients::deli_auth::v1_user_get::UserType;
  using V1UserGetRequest = deli_main::clients::deli_auth::v1_user_get::Request;
  using V1UserGetResponse200 = deli_main::clients::deli_auth::v1_user_get::Response200;
  using V1UserGetResponse400 = deli_main::clients::deli_auth::v1_user_get::Response400;

  inline std::string GetFieldType(const userver::formats::json::Value &elem) {
    if (elem.IsBool()) {
      return "bool";
    }
    if (elem.IsInt64()) {
      return "int";
    }
    if (elem.IsDouble()) {
      return "double";
    }
    if (elem.IsString()) {
      return "string";
    }
    if (elem.IsArray()) {
      return "array";
    }
    if (elem.IsObject()) {
      return "object";
    }
    throw std::runtime_error("Can't recognize type of object");
  }
}

namespace userver::formats::parse {

  inline V1UserGetUserType Parse(const userver::formats::json::Value &elem,
                                 userver::formats::parse::To<V1UserGetUserType>) {
    if (!elem.IsString()) {
      throw V1UserGetResponse400(
              fmt::format("Unexpected type received of field 'user_type', expected: string, got: {}",
                          GetFieldType(elem)));
    }
    const auto elem_as_str = elem.As<std::string>();
    if (elem_as_str == "customer") {
      return V1UserGetUserType::kCustomer;
    } else if (elem_as_str == "admin") {
      return V1UserGetUserType::kAdmin;
    } else if (elem_as_str == "courier") {
      return V1UserGetUserType::kCourier;
    }
    throw V1UserGetResponse400(
            fmt::format("Unexpected value received, expected: one of 'courier', 'admin', 'customer', got: {}",
                        elem_as_str));
  }

  inline V1UserGetResponse200 Parse(const userver::formats::json::Value &elem,
                                    userver::formats::parse::To<V1UserGetResponse200>) {
    if (!elem.HasMember("user_type")) {
      throw V1UserGetResponse400(
              fmt::format("Missing 'user_type' field in response"));
    }
    if (!elem.HasMember("login")) {
      throw V1UserGetResponse400(
              fmt::format("Missing 'login' field in response"));
    }
    if (!elem["login"].IsString()) {
      throw V1UserGetResponse400(
              fmt::format("Unexpected type received of field 'login', expected: string, got: {}",
                          GetFieldType(elem["login"])));
    }
    V1UserGetResponse200 response200{
            .login = elem["login"].As<std::string>(),
            .user_type = elem["user_type"].As<V1UserGetUserType>()
    };
    return response200;
  }

} // namespace userver::formats::parse


namespace userver::formats::serialize {

  inline json::Value Serialize(const V1UserGetRequest &value,
                               serialize::To<json::Value>) {
    if (!value.id.has_value() && !value.login.has_value()) {
      throw userver::formats::json::MemberMissingException("id or login must provided but no one was");
    } else if (value.id.has_value() && value.login.has_value()) {
      throw userver::formats::json::MemberMissingException("id or login must provided but only one of them");
    }

    userver::formats::json::ValueBuilder builder;
    if (value.id.has_value()) {
      builder["id"] = std::to_string(value.id.value());
    } else {
      builder["login"] = value.login.value();
    }
    return builder.ExtractValue();
  }

} // namespace userver::formats::serialize
