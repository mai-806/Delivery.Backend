#pragma once

#include <cinttypes>
#include <string>

namespace deli_main::views {

  struct Coordinate {
    double lon;
    double lat;
  };

  struct ErrorResponse {
    std::string message;
  };

  struct JustOkResponse {
    std::string message;
  };

  namespace v1::order::post {
    struct OrderCreationRequest {
      int64_t customer_id;
      Coordinate start;
      Coordinate finish;
    };

    struct OrderCreationResponse {
      int64_t order_id;
    };
  }
  namespace v1::order::change_status::post {
    struct OrderChangeStatusRequest {
      int64_t id;
      std::string status;
    };
  }
} // namespace deli_main::views
