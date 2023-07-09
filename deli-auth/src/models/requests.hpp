#pragma once

#include <models/models.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace deli_auth::models::requests {


  std::string DeleteAccessTokenWhereUserID(const userver::storages::postgres::ClusterPtr& cluster,
                                           const models::User &user);

  std::string SelectAccessTokenWhereUserID(const userver::storages::postgres::ClusterPtr& cluster,
                                           const models::User &user);

  std::string SelectLogin(const userver::storages::postgres::ClusterPtr& cluster,
                          const models::User &user);

} // namespace deli_auth::models::requests