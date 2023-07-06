#pragma once

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>

#include <views/objects/parsers.hpp>
#include <client/deli_auth_client.hpp>

namespace deli_main::views::v1::test_client::get {

  class Handler : public userver::server::handlers::HttpHandlerJsonBase {
  public:
    static constexpr std::string_view kName = "v1-test_client-get-handler";

    using Response400 = ErrorResponse;

    Handler(const userver::components::ComponentConfig &config,
            const userver::components::ComponentContext &component_context);

    userver::formats::json::Value HandleRequestJsonThrow(
            const userver::server::http::HttpRequest &, const userver::formats::json::Value &json,
            userver::server::request::RequestContext &) const override;

  private:
    const deli_auth::clients::DeliAuthClient &client_;
  };
} // namespace deli_main::views::v1::order::post
