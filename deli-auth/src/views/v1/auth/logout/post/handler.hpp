#pragma once

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>

#include <views/objects/parsers.hpp>

#include <components/requester.hpp>

namespace {
  std::string GenerationToken(int64_t user_id, int64_t expires_in);
}

namespace deli_auth::views::v1::auth::logout::post {

  class Handler : public userver::server::handlers::HttpHandlerJsonBase {
  public:
    static constexpr std::string_view kName = "v1-auth-logout-post-handler";

    using Request = LogoutRequest;
    using Response400 = ErrorResponse;
    using Response401 = ErrorResponse;
    using Response404 = ErrorResponse;
    using Response200 = LogoutResponse200;

    Handler(const userver::components::ComponentConfig &config,
            const userver::components::ComponentContext &component_context);

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest &, const userver::formats::json::Value &json,
        userver::server::request::RequestContext &) const override;

  private:
    const components::Requester &requester_;
  };
} // namespace deli_auth::views::v1::auth::logout::post