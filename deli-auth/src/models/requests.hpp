#pragma once

#include <models/models.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace deli_auth::models::requests {


  int64_t InsertUser(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::UserRegisterRequest &user);


} // namespace deli_auth::models::requests
