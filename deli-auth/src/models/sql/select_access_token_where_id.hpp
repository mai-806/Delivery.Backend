#pragma once


namespace deli_auth::models::sql {
  const constexpr char *kSelectTokensByID = "SELECT access_token, refresh_token, expires_in "
                                        "FROM deli_auth.bearer_tokens "
                                        "WHERE user_id=$1.id";

} // namespace deli_auth::models::sql
