#pragma once


namespace deli_main::models::sql {
  const constexpr char *kSelectCourier = "SELECT * "
                                         "FROM deli_main.couriers "
                                         "WHERE id = $1.id "
                                         "RETURNING login;";
} // namespace deli_main::models::sql
