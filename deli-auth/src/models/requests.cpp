#include "requests.hpp"

#include <models/sql/all.hpp>

namespace deli_auth::models::requests {

    UserGetResponse UserGet(const userver::storages::postgres::ClusterPtr& cluster,
                        const models::User &user) {
        const auto &result =
                cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                                 sql::kGetUser,
                                 user);
        return result.AsSingleRow<UserGetResponse>();
    }

} // namespace deli_auth::models::requests