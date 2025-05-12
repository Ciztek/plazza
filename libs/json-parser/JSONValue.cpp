#include "JSONValue.hpp"

auto JSON::JSONValue::to_string(size_t depth) const -> std::string
{
  std::string str;
  std::string indent(depth * 2, ' ');
  std::string new_indent((depth + 1) * 2, ' ');

  if (std::holds_alternative<std::nullptr_t>(stored))
    return "null";
  if (std::holds_alternative<bool>(stored))
    return std::get<bool>(stored) ? "true" : "false";
  if (std::holds_alternative<double>(stored))
    return std::to_string(std::get<double>(stored));
  if (std::holds_alternative<std::string>(stored))
    return "\"" + std::get<std::string>(stored) + "\"";
  if (std::holds_alternative<JsonArray>(stored)) {
    str.append("[\n");
    for (const auto &elem: std::get<JsonArray>(stored))
      str.append(new_indent).append(elem->to_string(depth + 1)).append(",\n");
    if (str.size() > 2)
      str.erase(str.size() - 2, 2);
    str.append("\n").append(indent).append("]");
    return str;
  }
  if (std::holds_alternative<JsonObject>(stored)) {
    str.append("{\n");
    for (const auto &[key, elem]: std::get<JsonObject>(stored))
      str.append(new_indent)
        .append(key)
        .append(": ")
        .append(elem->to_string(depth + 1))
        .append(",\n");
    if (str.size() > 2)
      str.erase(str.size() - 2, 2);
    str.append("\n").append(indent).append("}");
    return str;
  }
  __builtin_unreachable();
}
