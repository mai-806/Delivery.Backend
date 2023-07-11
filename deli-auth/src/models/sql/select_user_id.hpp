#pragma once


namespace deli_auth::models::sql {
  const constexpr char *kSelectUserID = "SELECT id FROM deli_auth.users WHERE login=$1.login";

} // namespace deli_auth::models::sql
