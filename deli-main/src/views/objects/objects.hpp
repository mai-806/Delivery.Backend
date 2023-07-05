#pragma once

#include <cinttypes>
#include <string>
#include <vector>

namespace deli_main::views {

  struct Coordinate {
    double lon;
    double lat;
  };

  struct ErrorResponse {
    std::string message;
  };
// new
  namespace v1::courier::get {
    struct CourierInfoRequest{
      int64_t courier_id;
    };
    struct CourierInfoResponse {
      int64_t courier_id;
      std::string login;
      std::vector<int64_t> orders_id;
    }
  }
//
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

} // namespace deli_main::views
