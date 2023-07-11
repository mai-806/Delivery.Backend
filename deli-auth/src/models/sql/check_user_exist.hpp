#pragma once


namespace deli_auth::models::sql {
    
  const constexpr char *kCheckUserExists = "SELECT EXISTS(SELECT 1 FROM deli_auth.users WHERE id = $1);";

} // namespace deli_auth::models::sql