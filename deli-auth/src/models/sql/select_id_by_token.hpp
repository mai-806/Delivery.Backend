#pragma once


namespace deli_auth::models::sql {
    const constexpr char *kSelectIdByToken = "SELECT user_id FROM deli_auth.bearer_tokens "
                                             "WHERE access_token = $1;";
} // namespace deli_auth::models::sql