#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/dynamic_config/source.hpp>
#include <userver/clients/http/client.hpp>
#include <userver/http/url.hpp>


namespace deli_auth::clients::components {

    class DeliAuthClient : public userver::components::LoggableComponentBase {
    public:
        static constexpr auto kName = "main-client";

        DeliAuthClient(const userver::components::ComponentConfig &config,
                  const userver::components::ComponentContext &context);

        const auto V1UserGet(const userver::formats::json::Value data){
          const auto url =
                  userver::http::MakeUrl("http://localhost:8080/v1/user", data);
          const auto response = http_client_.CreateRequest()
                  .get(url)
                  .retry(2)
                  .timeout(std::chrono::milliseconds{500})
                  .perform();
          response->raise_for_status();
          return formats::json::FromString(response->body_view());
        }

        const auto V1UserPatch(const userver::formats::json::Value data){
          const auto response = client_.CreateRequest()
                  .patch("http://localhost:8080/v1/user", data)
                  .perform();
          return formats::json::FromString(response->body_view());
        }

    private:
        userver::clients::http::Client client_;
    };

} // deli_main::client::components
