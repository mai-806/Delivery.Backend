#pragma once


namespace deli_auth::models::sql {
    const constexpr char *kGetUser = "INSERT INTO deli_auth.orders "
                                         "(start_point, end_point, status, customer, created_at, updated_at) "
                                         "VALUES "
                                         "($1.start_point, $1.end_point, $1.status, $1.customer, "
                                         "CURRENT_TIMESTAMP, CURRENT_TIMESTAMP) "
                                         "RETURNING id;";
} // namespace deli_auth::models::sql