#include "requests.hpp"

#include <models/sql/all.hpp>

namespace deli_auth::models::requests {

  int64_t UpdateUserPassword(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::User &user) {
    const auto &result =
            cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                             sql::kUpdateUserPassword,
                             user);
    return result.AsSingleRow<int64_t>();
  }
} // namespace deli_auth::models::requests
