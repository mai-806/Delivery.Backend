#include "requests.hpp"

#include <models/sql/all.hpp>
#include <common/exceptions.hpp>

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

  std::string DeleteAccessTokenWhereUserID(const userver::storages::postgres::ClusterPtr& cluster,
                                           const models::User &user) {
    const auto &result =
        cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                         sql::kDeleteAccessTokenWhereUserID,
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

  std::vector<models::BearerTokens> SelectBearerTokensByID(const userver::storages::postgres::ClusterPtr& cluster,
                                                           const models::User &user) {
    const auto &result =
      cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                       sql::kSelectTokensByID,
                       user);
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

    User GetUserByLogin(const userver::storages::postgres::ClusterPtr& cluster,
                        const std::string& login) {
        const auto &result =
                cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                 sql::kGetUserByLogin,
                                 login);
        if(result.IsEmpty())
            throw deli_auth::common::exceptions::UserNotFound("User not found!");
        return result.AsSingleRow<User>(userver::storages::postgres::kRowTag);
    }

    User GetUserById(const userver::storages::postgres::ClusterPtr& cluster,
                     const int64_t& id){
        const auto &result =
                cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                 sql::kGetUserById,
                                 id);
        if(result.IsEmpty())
            throw deli_auth::common::exceptions::UserNotFound("User not found!");
        return result.AsSingleRow<User>(userver::storages::postgres::kRowTag);
    }

} // namespace deli_auth::models::requests
