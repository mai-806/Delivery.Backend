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

  struct User {
    int64_t id;
    std::string login;
    std::string password;
    UserType user_type;
  };

  struct BearerTokens {
    int64_t id;
    int64_t user_id;
    std::string access_token;
    std::string refresh_token;
    int64_t expires_in;
  };


} // namespace deli_auth::models



namespace userver::storages::postgres::io {

  template<>
  struct CppToUserPg<deli_auth::models::UserType> : EnumMappingBase<deli_auth::models::UserType>  {
    static constexpr DBTypeName postgres_name = "deli_auth.user_type";
    static constexpr Enumerator enumerators[] {
      {EnumType::kAdmin,    "admin"},
      {EnumType::kCustomer, "customer"},
      {EnumType::kCourier, "courier"},};
  };

  template<>
  struct CppToUserPg<deli_auth::models::User> {
    static constexpr DBTypeName postgres_name = "deli_auth.user_v1";
  };

  template<>
  struct CppToUserPg<deli_auth::models::BearerTokens> {
    static constexpr DBTypeName postgres_name = "deli_auth.bearer_token_v1";
  };

} // namespace userver::storages::postgres::io
