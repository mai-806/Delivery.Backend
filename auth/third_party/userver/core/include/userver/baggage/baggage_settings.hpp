#pragma once

#include <unordered_set>

#include <userver/dynamic_config/snapshot.hpp>
#include <userver/dynamic_config/value.hpp>

USERVER_NAMESPACE_BEGIN

namespace baggage {

class BaggageSettings final {
 public:
  static BaggageSettings Parse(const dynamic_config::DocsMap& docs_map);

  std::unordered_set<std::string> allowed_keys;
  bool enabled;
};

constexpr dynamic_config::Key<BaggageSettings::Parse> kBaggageSettings;

bool ParseBaggageEnabled(const dynamic_config::DocsMap& docs_map);

constexpr dynamic_config::Key<ParseBaggageEnabled> kBaggageEnabled{};

}  // namespace baggage

USERVER_NAMESPACE_END
