#pragma once


namespace deli_main::models::sql {
  const constexpr char *kUpdateOrderStatus = "UPDATE deli_main.orders "
                                             "SET status = $1.status"
                                             "WHERE id = $1.id;";
} // namespace deli_main::models::sql
