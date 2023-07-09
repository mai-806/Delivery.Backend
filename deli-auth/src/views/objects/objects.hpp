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

  namespace v1::auth::login::post {
    struct TokenDto {
      std::string access_token;
      std::string refresh_token;
      int64_t expires_in;
      std::string token_type = "bearer";
    };

    struct AuthRequest{
      std::string login;
    };

    struct AuthResponse200{
      bool is_auth;
      std::string access_token;
    };
  }

} // namespace deli_auth::views
