#pragma once

#include <unordered_map>
#include <variant>
#include <vector>

#include "ErrorOr.hpp"

namespace JSON {
  struct JSONValue;

  using JSONArray = std::vector<JSONValue>;
  using JSONObject = std::unordered_map<std::string, JSONValue>;

  struct JSONValue {
    using Value = std::
      variant<std::nullptr_t, bool, double, std::string, JSONArray, JSONObject>;

    Value stored;

    JSONValue() : stored(nullptr) {}

    JSONValue(const std::string &str) : stored(str) {}

    JSONValue(double num) : stored(num) {}

    JSONValue(bool boolean) : stored(boolean) {}

    JSONValue(const JSONArray &array) : stored(array) {}

    JSONValue(const JSONObject &object) : stored(object) {}

    [[nodiscard]]
    auto to_string(size_t depth = 0) const -> std::string;

    template <typename T> [[nodiscard]] auto get() const -> ErrorOr<T>
    {
      auto *val = std::get_if<T>(&stored);
      if (val)
        return *val;
      return Error(
        "Invalid type" + std::string(typeid(T).name()) + " for "
        + to_string());
    }

    template <typename T>
    [[nodiscard]] auto get(const std::string &key) const -> ErrorOr<T>
    {
      const auto *obj = std::get_if<JSONObject>(&stored);
      if (!obj)
        return Error("Value is not a JSON object");
      auto it = obj->find(key);
      if (it == obj->end())
        return Error(key + " not found in JSON object");
      auto *val = std::get_if<T>(&it->second.stored);
      if (val)
        return *val;
      return Error(
        "Invalid type" + std::string(typeid(T).name()) + " for "
        + to_string());
    }

    template <typename T>
    [[nodiscard]] auto get(size_t idx) const -> ErrorOr<T>
    {
      const auto *obj = std::get_if<JSONArray>(&stored);
      if (!obj)
        return Error("Value is not a JSON object");
      auto *val = std::get_if<T>((*obj)[idx]);
      if (val)
        return *val;
      return Error(
        "Invalid type" + std::string(typeid(T).name()) + " for "
        + to_string());
    }
  };
}  // namespace JSON
