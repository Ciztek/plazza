#ifndef JSON_HPP_
#define JSON_HPP_

#include <filesystem>
#include <string>

#include "ErrorOr.hpp"
#include "JSONValue.hpp"

namespace JSON {
  class Parser {
  public:
    Parser(std::string const &str, size_t pos) : _str(str), _pos(pos) {}

    static auto
    load_from_string(std::string const &input) -> ErrorOr<JSONValue>;
    static auto load_from_file(const std::filesystem::path &filepath)
      -> ErrorOr<JSONValue>;

  private:
    auto eval() -> ErrorOr<JSONValue>;
    void consume_whitespace();

    auto parse_number() -> ErrorOr<JSONValue>;
    auto parse_literal() -> ErrorOr<JSONValue>;
    auto parse_value() -> ErrorOr<JSONValue>;

    auto parse_object() -> ErrorOr<JsonObject>;
    auto parse_array() -> ErrorOr<JsonArray>;
    auto parse_string() -> ErrorOr<std::string>;

    std::string const &_str;
    size_t _pos;
  };
}  // namespace JSON

#endif /* !JSON_HPP_ */
