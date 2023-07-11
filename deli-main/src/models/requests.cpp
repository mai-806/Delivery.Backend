#include "requests.hpp"

#include <models/sql/all.hpp>

namespace deli_main::models::requests {

  models::Order GetOrder(const userver::storages::postgres::ClusterPtr& cluster, const int64_t order_id) {
    const auto &result = cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                                          sql::kGetOrderInfo,
                                          order_id);
    if (result.IsEmpty()) throw userver::storages::postgres::RuntimeError("Order not found");

    return result.AsSingleRow<models::Order>(userver::storages::postgres::kRowTag);
  }

  int64_t InsertOrder(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::Order &order) {
    const auto &result =
            cluster->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                             sql::kInsertOrder,
                             order);
    return result.AsSingleRow<int64_t>();
  }

  std::vector<models::Order> GetOrders(const userver::storages::postgres::ClusterPtr& cluster,
                                     int64_t limit, int64_t offset) {
    const auto &result =
      cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                       sql::kGetOrders,
                       limit, offset);
    return result.AsContainer<std::vector<models::Order>>(userver::storages::postgres::kRowTag);
  }

  std::vector<int64_t> GetFreeCourierIds(const userver::storages::postgres::ClusterPtr& cluster,
                                         int64_t limit, int64_t offset) {
    const auto &result =
        cluster->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                         sql::kGetFreeCourierIds,
                         limit, offset);
    return result.AsContainer<std::vector<int64_t>>();
  }
} // namespace deli_main::models::requests
