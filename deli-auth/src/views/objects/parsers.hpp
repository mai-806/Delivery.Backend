#pragma once

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>

#include <views/objects/objects.hpp>

namespace userver::formats::parse {

  deli_auth::views::ErrorResponse
  Parse(const userver::formats::json::Value &elem,
        userver::formats::parse::To<deli_auth::views::ErrorResponse>);

  deli_auth::views::v1::auth::login::post::AuthRequest
  Parse(const userver::formats::json::Value &elem,
        userver::formats::parse::To<deli_auth::views::v1::auth::login::post::AuthRequest>);

  deli_auth::views::v1::auth::login::post::AuthResponse200
  Parse(const userver::formats::json::Value &elem,
        userver::formats::parse::To<deli_auth::views::v1::auth::login::post::AuthResponse200>);

    deli_auth::views::v1::auth::user::reset::post::UserResetRequest Parse(
            const userver::formats::json::Value &elem,
            userver::formats::parse::To<deli_auth::views::v1::auth::user::reset::post::UserResetRequest>);
  
    deli_auth::views::v1::auth::user::post::RegisterRequest Parse(
            const userver::formats::json::Value &elem,
            userver::formats::parse::To<deli_auth::views::v1::auth::user::post::RegisterRequest>);

} // namespace userver::formats::parse

namespace userver::formats::serialize {

  json::Value Serialize(const deli_auth::views::ErrorResponse &value,
                        serialize::To<json::Value>);

  json::Value Serialize(const deli_auth::views::v1::auth::login::post::AuthRequest &value,
                        serialize::To<json::Value>);

  json::Value Serialize(const deli_auth::views::v1::auth::login::post::AuthResponse200 &value,
                        serialize::To<json::Value>);

    json::Value Serialize(const deli_auth::views::ErrorResponse &value,
                          serialize::To<json::Value>);
  
    json::Value Serialize(const deli_auth::views::v1::auth::user::post::RegisterResponse &value,
                        serialize::To<json::Value>);
} // namespace userver::formats::serialize
