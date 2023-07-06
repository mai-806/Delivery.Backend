#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/dynamic_config/source.hpp>
#include <userver/clients/http/client.hpp>
#include <userver/clients/http/request.hpp>
#include <userver/http/url.hpp>
#include <userver/formats/json.hpp>
#include "userver/clients/http/component.hpp"
#include <userver/formats/parse/common_containers.hpp>


using Args = userver::http::Args;

namespace deli_auth::clients {

    class DeliAuthClient : public userver::components::LoggableComponentBase {
    public:
        static constexpr auto kName = "deli-auth-client";

        DeliAuthClient(const userver::components::ComponentConfig &config,
                       const userver::components::ComponentContext &context);

        userver::formats::json::Value V1UserGet(const userver::formats::json::Value &data) const;

        userver::formats::json::Value V1UserPatch(const std::string &data);

    private:
        userver::clients::http::Client &client_;
    };

} // deli_main::client::components
