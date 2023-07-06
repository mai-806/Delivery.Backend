#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/dynamic_config/source.hpp>
#include <userver/storages/postgres/cluster.hpp>


namespace deli_main::client::components {


    class DeliMainClient : public userver::components::LoggableComponentBase {
    public:
        static constexpr auto kName = "requester";

        Requester(const userver::components::ComponentConfig &config,
                  const userver::components::ComponentContext &context);

        auto V1UserGet();
        auto V1UserPatch();

    private:
        userver::storages::postgres::ClusterPtr client_;
    };


} // deli_main::components
