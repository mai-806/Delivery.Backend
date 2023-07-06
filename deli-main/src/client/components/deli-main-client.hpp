#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/dynamic_config/source.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/clients/http/client.hpp>

namespace deli_main::client::components {


    class DeliMainClient : public userver::components::LoggableComponentBase {
    public:
        static constexpr auto kName = "requester";

        DeliMainClient(const userver::components::ComponentConfig &config,
                  const userver::components::ComponentContext &context);

        const auto V1UserGet();
        const auto V1UserPatch();

    private:
        userver::clients::http::Client client_;
    };


} // deli_main::components
