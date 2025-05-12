#include "../Exception.hpp"
#include "JSON.hpp"

auto Parser::JSON::Parser::array(const std::string &raw_data, size_t &pos)
  -> JSON
{
  JsonArray array;

  if (raw_data[pos++] != '[')
    throw ParserException("Invalid array start");
  consume_whitespace(raw_data, pos);

  if (raw_data[pos] == ']') {
    ++pos;
    return array;
  }

  while (true) {
    consume_whitespace(raw_data, pos);
    array.push_back(std::make_shared<JSON>(Parser::value(raw_data, pos)));
    consume_whitespace(raw_data, pos);
    if (raw_data[pos] == ']') {
      ++pos;
      break;
    }
    if (raw_data[pos++] != ',')
      throw ParserException("Expected comma in array");
  }
  return array;
}
