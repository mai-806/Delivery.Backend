#pragma once

#include <cinttypes>
#include <string>

namespace deli_auth::views {

  struct ErrorResponse {
    std::string message;
  };

  namespace v1::auth::user::reset::post {
    struct UserResetRequest {
      int64_t id;
    };
  }

} // namespace deli_auth::views
