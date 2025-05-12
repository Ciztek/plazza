#include "../Exception.hpp"
#include "JSON.hpp"

namespace {
  auto
  parse_fraction(std::string &str, const std::string &raw_data, size_t &pos)
    -> std::string &
  {
    if (pos < raw_data.size() && raw_data[pos] != '.')
      return str;
    str += raw_data[pos++];
    if (pos >= raw_data.size() || !isdigit(raw_data[pos]))
      throw Parser::ParserException("Fractional part must have digits");
    while (pos < raw_data.size() && isdigit(raw_data[pos]))
      str += raw_data[pos++];
    return str;
  }

  auto
  parse_exponent(std::string &str, const std::string &raw_data, size_t &pos)
    -> std::string &
  {
    if (pos < raw_data.size()
        && (raw_data[pos] == 'e' || raw_data[pos] == 'E')) {
      str += raw_data[pos++];
      if (pos < raw_data.size()
          && (raw_data[pos] == '+' || raw_data[pos] == '-'))
        str += raw_data[pos++];
      if (pos >= raw_data.size() || !isdigit(raw_data[pos]))
        throw Parser::ParserException("Exponent must have digits");
      while (pos < raw_data.size() && isdigit(raw_data[pos]))
        str += raw_data[pos++];
    }
    return str;
  }
}  // namespace

auto Parser::JSON::Parser::number(const std::string &raw_data, size_t &pos)
  -> JSON
{
  std::string str;

  if (raw_data[pos] == '-')
    str += raw_data[pos++];

  if (pos >= raw_data.size() || !isdigit(raw_data[pos]))
    throw ParserException("Invalid number");
  if (raw_data[pos] == '0') {
    str += '0';
    ++pos;
    if (pos < raw_data.size() && isdigit(raw_data[pos]))
      throw ParserException("Invalid number");
  } else if (isdigit(raw_data[pos])) {
    while (pos < raw_data.size() && isdigit(raw_data[pos]))
      str += raw_data[pos++];
  } else {
    throw ParserException("Invalid number");
  }

  parse_fraction(str, raw_data, pos);
  parse_exponent(str, raw_data, pos);

  try {
    return std::stod(str);
  } catch (const std::out_of_range &e) {
    throw ParserException("Invalid number format");
  }
}
