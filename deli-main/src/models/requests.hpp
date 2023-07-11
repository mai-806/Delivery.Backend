#pragma once

#include <models/models.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace deli_main::models::requests {

  models::Order GetOrder(const userver::storages::postgres::ClusterPtr& cluster, int64_t order_id);

  int64_t InsertOrder(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::Order &order);

  std::vector<models::Order> GetOrders(const userver::storages::postgres::ClusterPtr& cluster,
                      int64_t limit, int64_t offset);

  std::vector<int64_t> GetFreeCourierIds(const userver::storages::postgres::ClusterPtr& cluster,
                                         int64_t limit, int64_t offset);
} // namespace deli_main::models::requests
