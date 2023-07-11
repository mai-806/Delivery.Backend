#pragma once


namespace deli_auth::models::sql {
  const constexpr char *kInsertUser = "INSERT INTO deli_auth.users "
                                      "(login, password, user_type, created_at, updated_at) "
                                      "VALUES ($1.login, $1.password, $1.user_type, "
                                      "CURRENT_TIMESTAMP, CURRENT_TIMESTAMP) "
                                      "ON CONFLICT (login) DO NOTHING "
                                      "RETURNING id;";
} // namespace deli_auth::models::sql
