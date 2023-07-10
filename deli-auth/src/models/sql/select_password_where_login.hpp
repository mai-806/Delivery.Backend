#pragma once


namespace deli_auth::models::sql {
  const constexpr char *kSelectPasswordByLogin = "SELECT password FROM deli_auth.users WHERE login=$1.login";

} // namespace deli_auth::models::sql
