#pragma once

#include <cinttypes>
#include <string>

namespace deli_auth::views {

  struct ErrorResponse {
    std::string message;
  };

  namespace v1::auth::register {
    struct RegisterRequest {
      std::string login;
      std::string password;

    };

  struct RegisterResponse {
    int64_t order_id;
  };
  }

} // namespace deli_auth::views
