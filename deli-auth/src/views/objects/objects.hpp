#pragma once

#include <cinttypes>
#include <string>

namespace deli_auth::views {
  enum UserType {
    CUSTOMER,
    COURIER,
    ADMIN,
  };
  struct ErrorResponse {
    std::string message;
  };

  namespace v1::auth::logout::post {
    struct TokenDto {
      std::string access_token;
      std::string refresh_token;
      int64_t expires_in;
      std::string token_type = "bearer";
    };

    struct LogoutRequest{
      int64_t id;
    };

    struct LogoutResponse200{
      int64_t id;
      std::string login;
      bool is_auth;
    };
  }

} // namespace deli_auth::views