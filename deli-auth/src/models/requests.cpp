#include "requests.hpp"

#include <models/sql/all.hpp>

namespace deli_auth::models::requests {

  bool CheckUserExists(const userver::storages::postgres::ClusterPtr& cluster,
                       int64_t user_id) {
    const auto &result =
            cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                             sql::kCheckUserExists,
                             user_id);
    return result.AsSingleRow<bool>();
  }

  void UpdateUserLogin(const userver::storages::postgres::ClusterPtr& cluster,
                       int64_t user_id, const std::string &login) {
    cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                     sql::kUpdateUserLogin,
                     user_id, login);
  }

  void UpdateUserType(const userver::storages::postgres::ClusterPtr& cluster,
                      int64_t user_id, models::UserType user_type) {
    cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                     sql::kUpdateUserType,
                     user_id, user_type);
  }

  models::User GetUserById(const userver::storages::postgres::ClusterPtr& cluster,
                             int64_t user_id) {
    const auto &result =
            cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                             sql::kGetUserById,
                             user_id);
    return result.AsSingleRow<models::User>(userver::storages::postgres::kRowTag);
  }

} // namespace deli_auth::models::requests
