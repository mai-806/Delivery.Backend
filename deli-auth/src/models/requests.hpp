#pragma once

#include <models/models.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace deli_auth::models::requests {

  int64_t UpdateUserPassword(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::User &user);

} // namespace deli_auth::models::requests
