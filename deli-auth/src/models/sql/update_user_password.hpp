#pragma once


namespace deli_auth::models::sql {
    const constexpr char *kUpdateUserPassword = "UPDATE deli_auth.users "
                                                "SET password = $1.password "
                                                "WHERE id = $1.id "
                                                "RETURNING id;";
} // namespace deli_auth::models::sql