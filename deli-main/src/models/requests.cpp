#include "requests.hpp"

#include <models/sql/all.hpp>

namespace deli_main::models::requests {

  int64_t InsertOrder(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::Order &order) {
    const auto &result =
            cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                             sql::kInsertOrder,
                             order);
    return result.AsSingleRow<int64_t>();
  }

  int64_t SelectCourier(const userver::storages::postgres::ClusterPtr& cluster,
                        const models::Courier &courier) {
    const auto &result =
            cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                             sql::kSelectCourier,
                             courier);
    return result.AsSingleRow<std::string>();
  }

  int64_t SelectOders(const userver::storages::postgres::ClusterPtr& cluster,
                        const models::Courier &courier) {
    const auto &result =
            cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                             sql::kSelectOders,
                             courier);
    return result.AsSingleRow<std::vector<int64_t>>();
  }

} // namespace deli_main::models::requests
