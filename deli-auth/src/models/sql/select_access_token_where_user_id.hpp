#pragma once


namespace deli_auth::models::sql {
  const constexpr char *kSelectAccessTokenWhereUserID = "SELECT access_token FROM deli_auth.bearer_tokens WHERE user_id=$1.id";

} // namespace deli_auth::models::sql