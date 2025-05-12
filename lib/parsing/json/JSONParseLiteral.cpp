#include "../Exception.hpp"
#include "JSON.hpp"

auto Parser::JSON::Parser::literal(const std::string &raw_data, size_t &pos)
  -> JSON
{
  if (raw_data.compare(pos, 4, "true") == 0) {
    pos += 4;
    return true;
  }
  if (raw_data.compare(pos, 5, "false") == 0) {
    pos += 5;
    return false;
  }
  if (raw_data.compare(pos, 4, "null") == 0) {
    pos += 4;
    return nullptr;
  }
  throw ParserException("Invalid literal");
}
