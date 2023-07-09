#pragma once

#include <models/models.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace deli_auth::models::requests {


  int64_t SelectUserID(const userver::storages::postgres::ClusterPtr& cluster,
                       const models::User &user);

  std::vector<models::BearerTokens> SelectBearerTokens(const userver::storages::postgres::ClusterPtr& cluster,
                            const models::BearerTokens &bearer_token);

  std::string SelectPassword(const userver::storages::postgres::ClusterPtr& cluster,
                            const models::User &user);

  int64_t InsertToken(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::BearerTokens &bearer_token);


} // namespace deli_auth::models::requests
