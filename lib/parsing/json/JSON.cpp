#include <cctype>
#include <fstream>

#include "../Exception.hpp"
#include "JSON.hpp"

Parser::JSON::JSON(const std::filesystem::path &path)
{
  std::string raw_data;
  std::string line;
  std::ifstream file;
  size_t pos = 0;

  if (!std::filesystem::exists(path))
    throw FileException("File does not exist");
  if (!std::filesystem::is_regular_file(path))
    throw FileException("Path is not a file");
  if (path.extension() != ".json")
    throw FileException("File is not a JSON file");

  file.open(path);
  if (!file.is_open())
    throw FileException("Failed to open file");
  while (std::getline(file, line))
    raw_data += line;
  file.close();

  if (raw_data.empty())
    return;

  value = Parser::value(raw_data, pos).value;
  Parser::consume_whitespace(raw_data, pos);
  if (pos != raw_data.size())
    throw ParserException("Extra characters after valid JSON");
}

auto Parser::JSON::Parser::
  consume_whitespace(const std::string &raw_data, size_t &pos) -> void
{
  static const std::string whitespace = " \t\n\r";

  while (
    pos < raw_data.size()
    && whitespace.find(raw_data[pos]) != std::string::npos)
    ++pos;
}

auto Parser::JSON::to_string(size_t depth) const -> std::string
{
  std::string str;
  std::string indent(depth * 2, ' ');
  std::string new_indent((depth + 1) * 2, ' ');

  if (std::holds_alternative<std::nullptr_t>(value))
    return "null";
  if (std::holds_alternative<bool>(value))
    return std::get<bool>(value) ? "true" : "false";
  if (std::holds_alternative<double>(value))
    return std::to_string(std::get<double>(value));
  if (std::holds_alternative<std::string>(value))
    return "\"" + std::get<std::string>(value) + "\"";
  if (std::holds_alternative<JsonArray>(value)) {
    str.append("[\n");
    for (const auto &elem: std::get<JsonArray>(value))
      str.append(new_indent).append(elem->to_string(depth + 1)).append(",\n");
    if (str.size() > 2)
      str.erase(str.size() - 2, 2);
    str.append("\n").append(indent).append("]");
    return str;
  }
  if (std::holds_alternative<JsonObject>(value)) {
    str.append("{\n");
    for (const auto &[key, elem]: std::get<JsonObject>(value))
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
  throw ParserException("Invalid value");
}

auto Parser::JSON::operator[](const std::string &key) const -> const JSON &
{
  if (std::holds_alternative<JsonObject>(value))
    return *std::get<JsonObject>(value).at(key);
  throw ParserException("Invalid access to JSON object");
}

auto Parser::JSON::operator[](size_t index) const -> const JSON &
{
  if (std::holds_alternative<JsonArray>(value))
    return *std::get<JsonArray>(value).at(index);
  throw ParserException("Invalid access to JSON array");
}
