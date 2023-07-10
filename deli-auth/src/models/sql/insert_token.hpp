#pragma once


namespace deli_auth::models::sql {
  const constexpr char *kInsertToken = "INSERT INTO deli_auth.bearer_tokens "
                                       "(user_id, access_token, refresh_token, expires_in) "
                                       "VALUES "
                                       "($1.user_id, $1.access_token, $1.refresh_token, $1.expires_in) "
                                       "RETURNING id;";
} // namespace deli_auth::models::sql
