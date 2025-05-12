#include "../Exception.hpp"
#include "JSON.hpp"

auto Parser::JSON::Parser::object(const std::string &raw_data, size_t &pos)
  -> JSON
{
  JsonObject object;

  if (raw_data[pos++] != '{')
    throw ParserException("Invalid object start");
  consume_whitespace(raw_data, pos);

  if (raw_data[pos] == '}') {
    ++pos;
    return object;
  }

  while (true) {
    consume_whitespace(raw_data, pos);
    auto key = string(raw_data, pos);
    consume_whitespace(raw_data, pos);
    if (raw_data[pos++] != ':')
      throw ParserException("Expected colon in object");
    consume_whitespace(raw_data, pos);
    object[key.to_string()] = std::
      make_shared<JSON>(Parser::value(raw_data, pos));
    consume_whitespace(raw_data, pos);
    if (raw_data[pos] == '}') {
      ++pos;
      break;
    }
    if (raw_data[pos++] != ',')
      throw ParserException("Expected comma in object");
  }
  return object;
}

