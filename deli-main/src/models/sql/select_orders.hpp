#pragma once


namespace deli_main::models::sql {
  const constexpr char *kSelectOrders = "SELECT * "
                                       "FROM deli_main.orders "
                                       "WHERE courier = $1.id "
                                       "RETURNING id;";
} // namespace deli_main::models::sql
