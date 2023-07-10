#pragma once

#include <models/models.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace deli_auth::models::requests {

  std::string DeleteAccessTokenWhereUserID(const userver::storages::postgres::ClusterPtr& cluster,
                                           const models::User &user);

  std::string SelectLogin(const userver::storages::postgres::ClusterPtr& cluster,
                          const models::User &user);

  int64_t SelectUserID(const userver::storages::postgres::ClusterPtr& cluster,
                       const models::User &user);

  std::vector<models::BearerTokens> SelectBearerTokens(const userver::storages::postgres::ClusterPtr& cluster,
                            const models::BearerTokens &bearer_token);
  std::vector<models::BearerTokens> SelectBearerTokensByID(const userver::storages::postgres::ClusterPtr& cluster,
                                                           const models::User &user);

  std::string SelectPassword(const userver::storages::postgres::ClusterPtr& cluster,
                            const models::User &user);

  int64_t InsertToken(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::BearerTokens &bearer_token);

  int64_t UpdateUserPassword(const userver::storages::postgres::ClusterPtr& cluster,
                             const models::User &user);

  int64_t SelectUserIdByToken(const userver::storages::postgres::ClusterPtr& cluster,
                              const std::string &token);

  int64_t InsertUser(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::UserRegisterRequest &user);

  User GetUserById(const userver::storages::postgres::ClusterPtr& cluster,
                   const int64_t& id);

  User GetUserByLogin(const userver::storages::postgres::ClusterPtr& cluster,
                      const std::string& login);

} // namespace deli_auth::models::requests
