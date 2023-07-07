#pragma once

#include <userver/formats/json/value.hpp>
#include <userver/storages/postgres/io/enum_types.hpp>
#include <userver/storages/postgres/io/json_types.hpp>


namespace deli_auth::models {

  enum class UserType {
    kUserTypeCustomer,
    kUserTypeCourier,
    kUserTypeAdmin
  };

  struct UserRegisterRequest{
    std::string login;
    std::string password;
    UserType user_type;
  }

  struct UserUpdateRequest {
    int64_t id;
    std::string login;
    UserType user_type;
  }

} // namespace deli_auth::models



namespace userver::storages::postgres::io {

  template<>
  struct CppToUserPg<deli_auth::models::UserType>
          : EnumMappingBase<deli_auth::models::UserType> {
    static constexpr DBTypeName postgres_name = "deli_auth.user_type";
    static constexpr EnumeratorList enumerators{
            {EnumType::kUserTypeCustomer, "customer"},
            {EnumType::kUserTypeCourier,  "courier"},
            {EnumType::kUserTypeAdmin,    "admin"}};
  };

  template<>
  struct CppToUserPg<deli_auth::models::UserUpdateRequest> {
    static constexpr DBTypeName postgres_name = "deli_auth.v1_user";
  };

} // namespace userver::storages::postgres::io
