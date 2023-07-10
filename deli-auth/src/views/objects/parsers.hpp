#pragma once

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>

#include <views/objects/objects.hpp>


namespace userver::formats::parse {

} // namespace userver::formats::parse

namespace userver::formats::serialize {

    json::Value Serialize(const deli_auth::views::ErrorResponse &value,
                          serialize::To<json::Value>);

    json::Value Serialize(const deli_auth::views::v1::user::get::UserGetResponse &value,
                          serialize::To<json::Value>);

} // namespace userver::formats::serialize