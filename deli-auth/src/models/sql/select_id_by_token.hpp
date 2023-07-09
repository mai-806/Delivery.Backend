#pragma once


namespace deli_auth::models::sql {
    const constexpr char *kSelectIdByToken = "SELECT id FROM deli_auth.bearer_tokens "
                                             "WHERE token = $1;";
} // namespace deli_auth::models::sql