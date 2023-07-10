#pragma once

#include <userver/formats/json/value.hpp>
#include <userver/storages/postgres/io/enum_types.hpp>
#include <userver/storages/postgres/io/json_types.hpp>


namespace deli_auth::models {

    enum class UserType {
        kCustomer,
        kCourier,
        kAdmin
    };

    struct User{
        int64_t id;
        std::string login;
        std::string password;
        UserType type;

    };

} // namespace deli_auth::models


namespace userver::storages::postgres::io {

    template<>
    struct CppToUserPg<deli_auth::models::User> {
        static constexpr DBTypeName postgres_name = "deli_auth.users";
    };

    template<>
    struct CppToUserPg<deli_auth::models::UserType>
            : EnumMappingBase<deli_auth::models::UserType> {
        static constexpr DBTypeName postgres_name = "deli_auth.user_type";
        static constexpr EnumeratorList enumerators{
                {EnumType::kCustomer,  "customer"},
                {EnumType::kCourier,    "courier"},
                {EnumType::kAdmin,     "admin"}};
    };

} // namespace userver::storages::postgres::io