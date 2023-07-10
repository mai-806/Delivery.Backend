#pragma once


namespace deli_main::models::sql {
const constexpr char *kGetFreeCourierIds = "SELECT id FROM deli_main.couriers "
                                           "WHERE status = 'free' "
                                           "ORDER BY id "
                                           "LIMIT $1 OFFSET $2;";
} // namespace deli_main::models::sql
