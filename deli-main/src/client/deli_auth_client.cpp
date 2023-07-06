#include "deli_auth_client.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/components/statistics_storage.hpp>
#include <userver/dynamic_config/storage/component.hpp>


namespace deli_auth::clients {

    DeliAuthClient::DeliAuthClient(const userver::components::ComponentConfig &config,
                                   const userver::components::ComponentContext &context) :
                                   LoggableComponentBase(config, context),
                                   client_(context.FindComponent<userver::components::HttpClient>().GetHttpClient()) {}

    userver::formats::json::Value DeliAuthClient::V1UserGet(const userver::formats::json::Value &data) const {
      const auto some_data = data.As<Args>();
      const auto url =
              userver::http::MakeUrl("http://localhost:38257/v1/user", some_data);
      const auto response = client_.CreateRequest()
              .get(url)
              .retry(2)
              .timeout(std::chrono::milliseconds{500})
              .perform();
      response->raise_for_status();
      return userver::formats::json::FromString(response->body_view());
    }

    userver::formats::json::Value DeliAuthClient::V1UserPatch(const std::string &data) {
      const auto response = client_.CreateRequest()
              .patch("http://localhost:38257/v1/user", data)
              .perform();
      return userver::formats::json::FromString(response->body_view());
    }

} // namespace deli_main::components
