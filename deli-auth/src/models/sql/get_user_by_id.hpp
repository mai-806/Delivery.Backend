#pragma once


namespace deli_auth::models::sql {
    
  const constexpr char *kUpdateUserLogin = "UPDATE deli_auth.users SET login = $2 WHERE id = $1;";
  
} // namespace deli_auth::models::sql