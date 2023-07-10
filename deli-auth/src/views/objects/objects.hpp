#pragma once

#include <cinttypes>
#include <string>

namespace deli_auth::views {

    enum class UserType{
        kCustomer,
        kCourier,
        kAdmin
    };

    struct ErrorResponse {
        std::string message;
    };

    namespace v1::user::get {

        struct  UserGetRequest{
            std::optional<int64_t> id;
            std::optional<std::string> login;
        };

        struct UserGetResponse {
            std::string login;
            UserType user_type;
        };
    }

} // namespace deli_auth::views