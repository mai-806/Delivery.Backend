#include "requests.hpp"

#include <models/sql/all.hpp>

namespace deli_auth::models::requests {

  int64_t InsertUser(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::UserRegisterRequest &user) {
    const auto &result =
            cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                             sql::kInsertUser,
                             user);

    if(result.IsEmpty())
      return -1;
    return result.AsSingleRow<int64_t>();
  }

  int64_t UpdateUserPassword(const userver::storages::postgres::ClusterPtr& cluster,
                               const models::User &user) {
        const auto &result =
                cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                 sql::kUpdateUserPassword,
                                 user);
        if (result.IsEmpty())
            return -1;
        return result.AsSingleRow<int64_t>();
    }

    int64_t SelectUserIdByToken(const userver::storages::postgres::ClusterPtr& cluster,
                               const std::string &token) {
        const auto &result =
                cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                 sql::kSelectUserIdByToken,
                                 token);
        if (result.IsEmpty())
            return -1;
        return result.AsSingleRow<int64_t>();
    }

} // namespace deli_auth::models::requests
