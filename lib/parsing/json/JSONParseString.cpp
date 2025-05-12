#include "../Exception.hpp"
#include "JSON.hpp"

namespace {

  auto parse_escape(std::string &str, const std::string &raw_data, size_t &pos)
    -> std::string &
  {
    static const std::string hex_chars = "0123456789abcdefABCDEF";

    ++pos;
    if (pos >= raw_data.size())
      throw Parser::ParserException("Unexpected end of string.");
    switch (raw_data[pos]) {
      case '"':
        str += '"';
        break;
      case '\\':
        str += '\\';
        break;
      case '/':
        str += '/';
        break;
      case 'b':
        str += '\b';
        break;
      case 'f':
        str += '\f';
        break;
      case 'n':
        str += '\n';
        break;
      case 'r':
        str += '\r';
        break;
      case 't':
        str += '\t';
        break;
      case 'u':
        ++pos;
        if (pos + 4 > raw_data.size())
          throw Parser::ParserException("Invalid unicode sequence in string.");
        if (raw_data.substr(pos, 4).find_first_not_of(hex_chars)
            != std::string::npos)
          throw Parser::ParserException("Invalid unicode sequence in string.");
        str += "\\u" + raw_data.substr(pos, 4);
        pos += 3;  // 3 because we will increment pos in the for loop
        break;
      default:
        throw Parser::ParserException("Invalid escape character in string.");
    }
    return str;
  }
}  // namespace

auto Parser::JSON::Parser::string(const std::string &raw_data, size_t &pos)
  -> JSON
{
  std::string str;

  if (raw_data[pos] != '"')
    throw ParserException("Non quoted string.");
  ++pos;
  while (pos < raw_data.size() && raw_data[pos] != '"') {
    if (raw_data[pos] == '\\')
      parse_escape(str, raw_data, pos);
    else
      str += raw_data[pos];
    ++pos;
  }
  if (pos >= raw_data.size() || raw_data[pos] != '"')
    throw ParserException("Unterminated string.");
  ++pos;
  return str;
}
