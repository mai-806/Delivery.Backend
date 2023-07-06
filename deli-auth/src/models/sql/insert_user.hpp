#pragma once


namespace deli_auth::models::sql {
  const constexpr char *kInsertOrder = "INSERT INTO deli_auth.users "
                                       "(login, password, user_type) "
                                       "VALUES "
                                       "($1.login, $1.password, $1.user_type, $1.customer) "
                                       "RETURNING id;";
} // namespace deli_auth::models::sql
