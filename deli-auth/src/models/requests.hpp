#pragma once

#include <models/models.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace deli_auth::models::requests {

    int64_t UpdateUserPassword(const userver::storages::postgres::ClusterPtr& cluster,
                               const models::User &user);

    int64_t SelectUserIdByToken(const userver::storages::postgres::ClusterPtr& cluster,
                            const std::string &token);

} // namespace deli_auth::models::requests