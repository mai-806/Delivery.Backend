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
  
  struct TokenDto {
      std::string access_token;
      std::string refresh_token;
      int64_t expires_in;
      std::string token_type = "bearer";
  };
    
  namespace v1::auth::login::post {
    struct AuthRequest{
      std::string login;
    };

    struct AuthResponse200{
      bool is_auth;
      std::string access_token;
    };
  }

  namespace v1::auth::logout::post {
    struct LogoutRequest{
      int64_t id;
    };

    struct LogoutResponse200{
      int64_t id;
      std::string login;
      bool is_auth;
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


    namespace v1::user::get {

        struct  UserGetRequest{
            std::optional<int64_t> id;
            std::optional<std::string> login;
        };

        struct UserGetResponse {
            std::string login;
            UserType user_type;
        };
    }

} // namespace deli_auth::views
