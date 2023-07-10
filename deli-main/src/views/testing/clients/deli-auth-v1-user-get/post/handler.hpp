#pragma once

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <clients/deli-auth/client.hpp>

#include <views/objects/parsers.hpp>


namespace deli_main::views::testing::deli_auth_v1_user_get::post {

  class Handler : public userver::server::handlers::HttpHandlerJsonBase {
  public:
    static constexpr std::string_view kName = "testing-clients-deli-auth-v1-user-get-post";


    Handler(const userver::components::ComponentConfig &config,
            const userver::components::ComponentContext &component_context);

    userver::formats::json::Value HandleRequestJsonThrow(
            const userver::server::http::HttpRequest &, const userver::formats::json::Value &json,
            userver::server::request::RequestContext &) const override;

  private:
    deli_main::clients::deli_auth::Client& deli_auth_client_;
  };
} // namespace deli_main::views::testing::deli_auth_v1_user_get::post
