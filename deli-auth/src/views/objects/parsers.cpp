
#include "parsers.hpp"

#include <vector>

#include <userver/logging/log.hpp>

namespace {
  enum class FieldType {
    kInt,
    kBool,
    kDouble,
    kString,
    kArray,
    kObject,
  };

  std::string ToString(FieldType type) {
    switch (type) {
      case FieldType::kInt:
        return "int";
      case FieldType::kBool:
        return "bool";
      case FieldType::kDouble:
        return "double";
      case FieldType::kString:
        return "string";
      case FieldType::kArray:
        return "array";
      case FieldType::kObject:
        return "object";
    }
  }

  FieldType GetFieldType(const userver::formats::json::Value &elem) {
    if (elem.IsBool()) {
      return FieldType::kBool;
    }
    if (elem.IsInt64()) {
      return FieldType::kInt;
    }
    if (elem.IsDouble()) {
      return FieldType::kDouble;
    }
    if (elem.IsString()) {
      return FieldType::kString;
    }
    if (elem.IsArray()) {
      return FieldType::kArray;
    }
    if (elem.IsObject()) {
      return FieldType::kObject;
    }
    throw std::runtime_error("Can't recognize type of object");
  }

  bool IsEqual(FieldType typel, FieldType typer) {
    if (typer != FieldType::kDouble)
      return typel == typer;
    const std::unordered_set<FieldType> d = {FieldType::kInt, FieldType::kDouble};
    return d.contains(typel);
  }


  using Types = std::unordered_map<std::string, FieldType>;
  using Keys = std::vector<std::string>;


  void CheckFields(const Keys &required_keys, const Keys &optional_keys, const Types &key_types,
                   const userver::formats::json::Value &elem) {
    for (const auto &key: required_keys) {
      if (!elem.HasMember(key)) {
        throw userver::formats::json::ParseException(
                fmt::format("Key '{}' is missing but required", key));
      }
      if (!IsEqual(GetFieldType(elem[key]), key_types.at(key))) {
        throw userver::formats::json::ParseException(
                fmt::format("Wrong type of key '{}', expected: '{}', got: '{}'",
                            key, ToString(key_types.at(key)), ToString(GetFieldType(elem[key]))));
      }
    }
    for (const auto &key: optional_keys) {
      if (elem.HasMember(key)) {
        if (!IsEqual(GetFieldType(elem[key]), key_types.at(key))) {
          throw userver::formats::json::ParseException(
                  fmt::format("Wrong type of key '{}', expected: '{}', got: '{}'",
                              key, ToString(GetFieldType(elem[key])), ToString(key_types.at(key))));
        }
      }
    }
  }

  template<typename T>
  T GetRequiredValue(const userver::formats::json::Value &elem, const std::string &key) {
    return elem[key].ConvertTo<T>();
  }

  template<typename T>
  std::optional<T> GetOptionalValue(const userver::formats::json::Value &elem, const std::string &key) {
    if (!elem.HasMember(key)) {
      return std::nullopt;
    }
    return std::optional(elem[key].ConvertTo<T>());
  }


}

namespace userver::formats::parse {


} // namespace userver::formats::parse


namespace userver::formats::serialize {


} // namespace userver::formats::serialize
