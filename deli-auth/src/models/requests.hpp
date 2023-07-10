#pragma once

#include <models/models.hpp>

#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/exceptions.hpp>

namespace deli_auth::models::requests {


    User GetUserById(const userver::storages::postgres::ClusterPtr& cluster,
                        const int64_t& id);

    User GetUserByLogin(const userver::storages::postgres::ClusterPtr& cluster,
                               const std::string& login);

} // namespace deli_auth::models::requests