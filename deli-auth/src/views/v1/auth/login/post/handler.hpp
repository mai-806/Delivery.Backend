#pragma once

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>

#include <views/objects/parsers.hpp>

#include <components/requester.hpp>

namespace deli_auth::views::v1::auth::login::post {

  class Handler : public userver::server::handlers::HttpHandlerJsonBase {
  public:
    static constexpr std::string_view kName = "v1-auth-login-post-handler";

    using Request = AuthRequest;
    using Response400 = ErrorResponse;
    using Response404 = ErrorResponse;
    using Response200 = AuthResponse200;

    Handler(const userver::components::ComponentConfig &config,
            const userver::components::ComponentContext &component_context);

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest &, const userver::formats::json::Value &json,
        userver::server::request::RequestContext &) const override;

  private:
    const components::Requester &requester_;
  };
} // namespace deli_auth::views::v1::auth::login::post
