#include "requests.hpp"

#include <models/sql/all.hpp>

namespace deli_auth::models::requests {

  int64_t InsertUser(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::UserRegisterRequest &user) {
    const auto &result =
            cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                             sql::kInsertUser,
                             order);
    return result.AsSingleRow<int64_t>();
  }

} // namespace deli_auth::models::requests
