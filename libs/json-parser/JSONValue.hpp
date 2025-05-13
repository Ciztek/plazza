#pragma once

#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

namespace JSON {
  struct JSONValue;

  using JsonArray = std::vector<std::shared_ptr<JSONValue>>;
  using JsonObject = std::
    unordered_map<std::string, std::shared_ptr<JSONValue>>;

  struct JSONValue {
    using Value = std::
      variant<std::nullptr_t, bool, double, std::string, JsonArray, JsonObject>;

    Value stored;

    JSONValue() : stored(nullptr) {}

    JSONValue(const std::string &str) : stored(str) {}

    JSONValue(double num) : stored(num) {}

    JSONValue(bool boolean) : stored(boolean) {}

    JSONValue(const JsonArray &array) : stored(array) {}

    JSONValue(const JsonObject &object) : stored(object) {}

    [[nodiscard]]
    auto to_string(size_t depth = 0) const -> std::string;
  };
}  // namespace JSON
