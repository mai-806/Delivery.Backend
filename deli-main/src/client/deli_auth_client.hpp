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

namespace deli_auth::clients::components {

    class DeliAuthClient : public userver::components::LoggableComponentBase {
    public:
        static constexpr auto kName = "main-client";

        DeliAuthClient(const userver::components::ComponentConfig &config,
                       const userver::components::ComponentContext &context);


        //userver::formats::json::Value -> Args
        auto V1UserGet(const userver::formats::json::Value &data) const {
          const auto some_data = data.As<Args>();
          const auto url =
                  userver::http::MakeUrl("http://localhost:8080/v1/user", some_data);
          const auto response = client_.CreateRequest()
                  .get(url)
                  .retry(2)
                  .timeout(std::chrono::milliseconds{500})
                  .perform();
          response->raise_for_status();
          return userver::formats::json::FromString(response->body_view());
        }

        // userver::formats::json::Value -> string
        auto V1UserPatch(const std::string &data) {
          const auto response = client_.CreateRequest()
                  .patch("http://localhost:8080/v1/user", std::move(data))
                  .perform();
          return userver::formats::json::FromString(response->body_view());
        }

    private:
        userver::clients::http::Client &client_;
    };

} // deli_main::client::components
