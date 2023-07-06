#include "deli-main-client.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/components/statistics_storage.hpp>
#include <userver/dynamic_config/storage/component.hpp>
#include <userver/storages/postgres/component.hpp>


namespace deli_main::client::components {

    DeliMainClient::DeliMainClient(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context) : LoggableComponentBase(config, context),
              client_(context.FindComponent<userver::components::HttpClient>().GetHttpClient()) {}

} // namespace deli_main::components
