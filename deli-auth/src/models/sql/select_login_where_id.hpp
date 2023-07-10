#pragma once


namespace deli_auth::models::sql {
  const constexpr char *kSelectLogin = "SELECT login FROM deli_auth.users WHERE id=$1.id";

} // namespace deli_auth::models::sql