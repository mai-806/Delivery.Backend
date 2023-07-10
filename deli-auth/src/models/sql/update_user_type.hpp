#pragma once


namespace deli_auth::models::sql {
    
  const constexpr char *kGetUserById = "SELECT id, login, password, user_type FROM deli_auth.users WHERE id = $1;";

} // namespace deli_auth::models::sql