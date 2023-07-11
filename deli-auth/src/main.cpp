#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include <common/consts.hpp>
#include <components/requester.hpp>
#include <views/v1/user/patch/handler.hpp>
#include <views/v1/auth/user/reset/post/handler.hpp>
#include <views/v1/auth/user/post/handler.hpp>
#include <views/v1/auth/login/post/handler.hpp>
#include <views/v1/auth/logout/post/handler.hpp>
#include <views/v1/user/get/handler.hpp>


int main(int argc, char *argv[]) {
    auto component_list = userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .Append<userver::components::HttpClient>()
            .Append<userver::server::handlers::TestsControl>()
            .Append<deli_auth::components::Requester>()
            .Append<deli_auth::views::v1::auth::user::reset::post::Handler>()
            .Append<deli_auth::views::v1::auth::user::post::Handler>()
            .Append<deli_auth::views::v1::auth::login::post::Handler>()
            .Append<deli_auth::views::v1::auth::logout::post::Handler>()
            .Append<deli_auth::views::v1::user::get::Handler>()
            .Append<deli_auth::views::v1::user::patch::Handler>()
            .Append<userver::clients::dns::Component>()
            .Append<userver::components::Postgres>(deli_auth::common::consts::kPgClusterName);

    return userver::utils::DaemonMain(argc, argv, component_list);
}