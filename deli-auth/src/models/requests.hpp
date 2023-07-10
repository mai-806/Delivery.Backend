#pragma once

#include <models/models.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace deli_auth::models::requests {
  
  bool CheckUserExists(const userver::storages::postgres::ClusterPtr& cluster,
                       int64_t user_id);

  void UpdateUserLogin(const userver::storages::postgres::ClusterPtr& cluster,
                       int64_t user_id, const std::string &login);

  void UpdateUserType(const userver::storages::postgres::ClusterPtr& cluster,
                      int64_t user_id, models::UserType user_type);

  models::User GetUserById(const userver::storages::postgres::ClusterPtr& cluster,
                                        int64_t user_id);


} // namespace deli_auth::models::requests
