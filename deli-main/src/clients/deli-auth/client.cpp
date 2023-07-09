#include "client.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/components/statistics_storage.hpp>
#include <userver/dynamic_config/storage/component.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/http/url.hpp>


namespace deli_main::clients::deli_auth {

  Client::Client(const userver::components::ComponentConfig &config,
                 const userver::components::ComponentContext &context)
          : LoggableComponentBase(config, context),
            http_client_(context.FindComponent<userver::components::HttpClient>().GetHttpClient()),
            v1_user_get_url_(config["v1-user-get-url"].As<std::string>()) {}


  v1_user_get::Response200 Client::V1UserGet(const v1_user_get::Request &request) try {

    const auto json_request =
            userver::formats::serialize::Serialize(request,
                                                   userver::formats::serialize::To<userver::formats::json::Value>());

    const auto url =
            userver::http::MakeUrl(v1_user_get_url_, json_request.As<userver::http::Args>());
    LOG_DEBUG() << fmt::format("start request to: '{}'", url);

    auto response = http_client_.CreateRequest()
            .get(url)
            .retry(2)
            .timeout(std::chrono::milliseconds{500})
            .perform();

    response->raise_for_status();

    const auto response_json =
            userver::formats::json::FromString(response->body_view());

    return userver::formats::parse::Parse(
            response_json,
            userver::formats::parse::To<v1_user_get::Response200>());
  } catch (const userver::clients::http::HttpClientException &exception) {
    
    LOG_WARNING() << fmt::format("got {} response status from deli-auth service", exception.code());

    if (exception.code() == 400) {
      throw v1_user_get::Response400(exception.what());
    } else if (exception.code() == 404) {
      throw v1_user_get::Response404(exception.what());
    }
    throw v1_user_get::Response400(exception.what());
  }


  userver::yaml_config::Schema Client::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<userver::components::LoggableComponentBase>(R"(
type: object
description: deli-auth client
additionalProperties: false
properties:
    v1-user-get-url:
        type: string
        description: deli-auth v1/user get handler url
)");
  }

} // namespace deli_main::clients::deli_auth


