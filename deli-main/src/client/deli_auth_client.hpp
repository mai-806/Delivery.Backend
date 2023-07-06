#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/dynamic_config/source.hpp>
#include <userver/clients/http/client.hpp>

namespace deli_auth::clients::components {

    class DeliAuthClient : public userver::components::LoggableComponentBase {
    public:
        static constexpr auto kName = "main-client";

        DeliAuthClient(const userver::components::ComponentConfig &config,
                  const userver::components::ComponentContext &context);

        const auto V1UserGet();
        const auto V1UserPatch();

    private:
        userver::clients::http::Client client_;
    };

} // deli_main::client::components
