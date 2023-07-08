#pragma once

#include <cinttypes>
#include <string>

namespace deli_auth::views {

  struct ErrorResponse {
    std::string message;
  };

  namespace v1::user::patch {
    struct UserUpdateRequest {
      int64_t id;
      std::variant<std::monostate, std::string> login;
      std::variant<std::monostate, std::string> userType;
    };

    struct UserUpdateResponse200 {
      int64_t id;
      std::string login;
      std::string user_type;
    };
  };

}; // namespace deli_auth::views
