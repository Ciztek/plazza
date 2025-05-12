#include "../Exception.hpp"
#include "JSON.hpp"

auto Parser::JSON::Parser::value(const std::string &raw_data, size_t &pos)
  -> JSON
{
  consume_whitespace(raw_data, pos);
  if (pos >= raw_data.size())
    throw ParserException("Unexpected end of input");

  if (raw_data[pos] == '"')
    return string(raw_data, pos);
  if (isdigit(raw_data[pos]) || raw_data[pos] == '-')
    return number(raw_data, pos);
  if (raw_data[pos] == '{')
    return object(raw_data, pos);
  if (raw_data[pos] == '[')
    return array(raw_data, pos);
  if (isalpha(raw_data[pos]))
    return literal(raw_data, pos);

  throw ParserException("Invalid value");
}
