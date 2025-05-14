#include <filesystem>
#include <fstream>
#include <string>

#include "ErrorOr.hpp"
#include "JSONParser.hpp"
#include "JSONValue.hpp"

namespace JSON {

  auto Parser::load_from_string(std::string const &input) -> ErrorOr<JSONValue>
  {
    Parser p(input, 0);

    return p.eval();
  }

  auto Parser::load_from_file(const std::filesystem::path &filepath)
    -> ErrorOr<JSONValue>
  {
    MUST(std::filesystem::exists(filepath), "File does not exist");
    MUST(std::filesystem::is_regular_file(filepath), "File does not exist");
    MUST(filepath.extension() == ".json", "File is not a JSON file");

    std::ifstream file;

    file.open(filepath);
    MUST(file.is_open(), "Failed to open the file");

    std::stringstream buffer;
    buffer << file.rdbuf();

    MUST(!file.fail(), "Failed to read file content");
    file.close();
    return Parser::load_from_string(buffer.str());
  }

  void Parser::consume_whitespace()
  {
    static const std::string whitespace = " \t\n\r";

    while (
      _pos < _str.size() && whitespace.find(_str[_pos]) != std::string::npos)
      ++_pos;
  }

  auto Parser::eval() -> ErrorOr<JSONValue>
  {
    consume_whitespace();

    switch (_str[_pos]) {
      case '{':
        Log::debug << "parsing json obj";
        return JSONValue(TRY(parse_object()));
      case '[':
        Log::debug << "parsing json array";
        return JSONValue(TRY(parse_array()));
      case '"':
        Log::debug << "parsing json string";
        return JSONValue(TRY(parse_string()));
      case '-':
      case '0' ... '9':
        Log::debug << "parsing json number";
        return parse_number();
      case 'f':
      case 't':
      case 'n':
        Log::debug << "parsing json lit";
        return parse_literal();
      default:
        std::string err = "unexpected character [";
        err += _str[_pos];
        err += "]";

        return Error(err);
    }
    __builtin_unreachable();
  }
}  // namespace JSON
