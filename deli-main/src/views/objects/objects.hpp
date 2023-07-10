#pragma once

#include <cinttypes>
#include <string>

namespace deli_main::views {

  struct Coordinate {
    double lon;
    double lat;
  };

  enum class OrderStatus {
    kOrderStatusNew,
    kOrderStatusWaiting,
    kOrderStatusInProgress,
    kOrderStatusDelivered,
    kOrderStatusCanceled
  };

  struct ErrorResponse {
    std::string message;
  };

  struct OrderDto {
    int64_t order_id;
    int64_t customer_id;
    Coordinate start_point;
    Coordinate end_point;
    OrderStatus status;
    std::optional<int64_t> courier_id;
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

  namespace v1::orders::get {
      struct GetOrdersResponse {
        std::vector<OrderDto> orders;
      };
  }

  namespace v1::couriers::available::get {
    struct GetAvailableCouriersResponse {
      std::vector<int64_t> courier_ids;
    };
  }

} // namespace deli_main::views
