#include "requests.hpp"

#include <models/sql/all.hpp>

namespace deli_auth::models::requests {

  int64_t SelectUserID(const userver::storages::postgres::ClusterPtr& cluster,
                     const models::User &user) {
    const auto &result =
        cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                         sql::kSelectUserID,
                         user);
    return result.AsSingleRow<int64_t>();
  }

  std::vector<models::BearerTokens> SelectBearerTokens(const userver::storages::postgres::ClusterPtr& cluster,
                  const models::BearerTokens &bearer_token) {
    const auto &result =
        cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                         sql::kSelectTokens,
                         bearer_token);
    return result.AsContainer<std::vector<models::BearerTokens>>
    (userver::storages::postgres::kRowTag);
  }

  std::string SelectPassword(const userver::storages::postgres::ClusterPtr& cluster,
                            const models::User &user) {
    const auto &result =
        cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                         sql::kSelectPasswordByLogin,
                         user);
    return result.AsSingleRow<std::string>();
  }

  int64_t InsertToken(const userver::storages::postgres::ClusterPtr& cluster,
                             const models::BearerTokens &bearer_token) {
    const auto &result =
        cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                         sql::kInsertToken,
                         bearer_token);
    return result.AsSingleRow<int64_t>();
  }

} // namespace deli_auth::models::requests
