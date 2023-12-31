#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include <common/consts.hpp>
#include <components/requester.hpp>
#include <clients/deli-auth/client.hpp>


#include <views/v1/order/post/handler.hpp>
#include <views/v1/order/get/handler.hpp>
#include <views/v1/orders/get/handler.hpp>
#include <views/testing/clients/deli-auth-v1-user-get/post/handler.hpp>
#include <views/v1/couriers/available/get/handler.hpp>


int main(int argc, char *argv[]) {
  auto component_list = userver::components::MinimalServerComponentList()
          .Append<userver::server::handlers::Ping>()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::components::HttpClient>()
          .Append<userver::server::handlers::TestsControl>()
          .Append<deli_main::components::Requester>()
          .Append<deli_main::views::v1::order::post::Handler>()
          .Append<deli_main::views::v1::orders::get::Handler>()
          .Append<deli_main::views::v1::couriers::available::get::Handler>()
          .Append<deli_main::views::v1::order::get::Handler>()
          .Append<userver::clients::dns::Component>()
          .Append<userver::components::Postgres>(deli_main::common::consts::kPgClusterName)
          .Append<deli_main::views::testing::deli_auth_v1_user_get::post::Handler>()
          .Append<deli_main::clients::deli_auth::Client>();


  return userver::utils::DaemonMain(argc, argv, component_list);
}
