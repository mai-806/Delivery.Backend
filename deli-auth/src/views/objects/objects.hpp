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

  namespace v1::auth::user::reset::post {
    struct UserResetRequest {
      int64_t id;
    };
  }
  
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
