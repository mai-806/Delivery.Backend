#pragma once


namespace deli_main::models::sql {
  const constexpr char *kGetOrderInfo = "SELECT * FROM deli_main.orders "
                                        "WHERE id=$1";
}// namespace deli_main::models::sql
