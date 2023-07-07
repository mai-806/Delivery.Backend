#pragma once


namespace deli_auth::models::sql {
  const constexpr char *kUpdateUserPassword = "UPDATE deli_auth.users "
                                              "SET password = $2 "
                                              "WHERE id = $1;";
} // namespace deli_auth::models::sql
