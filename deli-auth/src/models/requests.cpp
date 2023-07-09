#include "requests.hpp"

#include <models/sql/all.hpp>

namespace deli_auth::models::requests {

  std::string DeleteAccessTokenWhereUserID(const userver::storages::postgres::ClusterPtr& cluster,
                                           const models::User &user) {
    const auto &result =
        cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                         sql::kDeleteAccessTokenWhereUserID,
                         user);
    return result.AsSingleRow<std::string>();
  }

  std::string SelectAccessTokenWhereUserID(const userver::storages::postgres::ClusterPtr& cluster,
                                           const models::User &user) {
    const auto &result =
        cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                         sql::kSelectAccessTokenWhereUserID,
                         user);
    return result.AsSingleRow<std::string>();
  }

  std::string SelectLogin(const userver::storages::postgres::ClusterPtr& cluster,
                          const models::User &user) {
    const auto &result =
        cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                         sql::kSelectLogin,
                         user);
    return result.AsSingleRow<std::string>();
  }

} // namespace deli_auth::models::requests