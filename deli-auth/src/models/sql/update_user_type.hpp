#pragma once


namespace deli_auth::models::sql {
    
  const constexpr char *kUpdateUserType = "UPDATE deli_auth.users SET user_type = $2 WHERE id = $1;";

} // namespace deli_auth::models::sql