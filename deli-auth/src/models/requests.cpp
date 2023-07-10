#include "requests.hpp"
#include <common/exeption.hpp>
#include <models/sql/all.hpp>

namespace deli_auth::models::requests {

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


} // namespace deli_auth::models::requests