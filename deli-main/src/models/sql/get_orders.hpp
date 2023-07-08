#pragma once


namespace deli_main::models::sql {
const constexpr char *kGetOrders = "SELECT * FROM deli_main.orders "
                                   "LIMIT $1 OFFSET $2;";
} // namespace deli_main::models::sql
