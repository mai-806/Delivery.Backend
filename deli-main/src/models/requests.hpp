#pragma once

#include <models/models.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <string>
#include <vector>

namespace deli_main::models::requests {


  int64_t InsertOrder(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::Order &order);

  int64_t SelectCourier(const userver::storages::postgres::ClusterPtr& cluster,
                     const models::Courier &courier);

  int64_t SelectOrders(const userver::storages::postgres::ClusterPtr& cluster,
                      const models::Courier &courier);
} // namespace deli_main::models::requests
