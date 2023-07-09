#pragma once

#include <cinttypes>
#include <string>

namespace deli_auth::views {

  enum class UserType {
    kUserTypeCustomer,
    kUserTypeCourier,
    kUserTypeAdmin
  };

  struct ErrorResponse {
    std::string message;
  };

  namespace v1::auth::user::post {
    struct RegisterRequest {
      std::string login;
      UserType user_type;
    };

    struct RegisterResponse {
      int64_t id;
    };
  } // namespace v1::auth::user::post

} // namespace deli_auth::views
