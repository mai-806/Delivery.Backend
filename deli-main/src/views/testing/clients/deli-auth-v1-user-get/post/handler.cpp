#include "handler.hpp"

#include <models/models.hpp>


namespace deli_main::views::testing::deli_auth_v1_user_get::post {

  Handler::Handler(const userver::components::ComponentConfig &config,
                   const userver::components::ComponentContext &component_context) :
          userver::server::handlers::HttpHandlerJsonBase(config, component_context),
          deli_auth_client_(component_context.FindComponent<deli_main::clients::deli_auth::Client>()) {}


  userver::formats::json::Value Handler::HandleRequestJsonThrow(
          const userver::server::http::HttpRequest &request, const userver::formats::json::Value &json,
          userver::server::request::RequestContext &) const try {

    const auto id = json["id"].As<int64_t>();
    clients::deli_auth::v1_user_get::Request request1;
    request1.id = id;

    const auto response = deli_auth_client_.V1UserGet(request1);

    request.SetResponseStatus(userver::server::http::HttpStatus::kOk);

    userver::formats::json::ValueBuilder response200;
    response200["login"] = response.login;
    response200["user_type"] = ToString(response.user_type);
    return response200.ExtractValue();

  } catch (const userver::formats::json::ParseException &exception) {
    request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
    return {};
  } catch (const clients::deli_auth::v1_user_get::Response404 &exception) {
    request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
    return {};
  } catch (const clients::deli_auth::v1_user_get::Response400 &exception) {
    request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
    return {};
  }

} // namespace deli_main::views::testing::deli_auth_v1_user_get::post
