#include <unordered_set>
#include "ErrorOr.hpp"
#include "JSONParser.hpp"
#include "StringMacros.h"

namespace {
  auto
  parse_fraction(std::string &str, const std::string &raw_data, size_t &pos)
    -> MaybeError
  {
    if (pos < raw_data.size() && raw_data[pos] != '.')
      return {};
    str += raw_data[pos++];
    if (pos >= raw_data.size() || !isdigit(raw_data[pos]))
      return Error("Fractional part must have digits");
    while (pos < raw_data.size() && isdigit(raw_data[pos]))
      str += raw_data[pos++];
    return {};
  }

  auto
  parse_exponent(std::string &str, const std::string &raw_data, size_t &pos)
    -> MaybeError
  {
    if (pos < raw_data.size()
        && (raw_data[pos] == 'e' || raw_data[pos] == 'E')) {
      str += raw_data[pos++];
      if (pos < raw_data.size()
          && (raw_data[pos] == '+' || raw_data[pos] == '-'))
        str += raw_data[pos++];
      if (pos >= raw_data.size() || !isdigit(raw_data[pos]))
        return Error("Exponent must have digits");
      while (pos < raw_data.size() && isdigit(raw_data[pos]))
        str += raw_data[pos++];
    }
    return {};
  }

  auto parse_escape(std::string &str, const std::string &raw_data, size_t &pos)
    -> MaybeError
  {
    static const std::string hex_chars = "0123456789abcdefABCDEF";

    ++pos;
    MUST(pos < raw_data.size(), "unexpected end of string");
    switch (raw_data[pos]) {

#define MAP(in, out) case in: str += (out); break
      MAP('"', '"');
      MAP('\\', '\\');
      MAP('/', '/');
      MAP('b', '\b');
      MAP('f', '\f');
      MAP('n', '\n');
      MAP('r', '\r');
      MAP('t', '\t');
#undef MAP
      case 'u':
        ++pos;
        MUST(pos + 4 < raw_data.size(), "invalid unicode sequence in string");
        MUST(
          raw_data.substr(pos, 4).find_first_not_of(hex_chars)
            == std::string::npos,
          "invalid unicode sequence in string");
        str += "\\u" + raw_data.substr(pos, 4);
        pos += 3;  // 3 because we will increment pos in the for loop
        break;
      default:
        return Error("invalid escape character in string");
    }
    return {};
  }
}  // namespace

namespace JSON {

  auto Parser::parse_number() -> ErrorOr<JSONValue>
  {
    std::string s;

    if (_str[_pos] == '-')
      s += _str[_pos++];

    MUST(_pos < _str.size(), "out of range");
    MUST(isdigit(_str[_pos]), "invalid number");

    if (_str[_pos] == '0') {
      s += '0';
      ++_pos;

      if (_pos < _str.size())
        MUST(!isdigit(_str[_pos]), "invalid number");
    } else {
      while (_pos < _str.size() && isdigit(_str[_pos]))
        s += _str[_pos++];
    }

    TRY(parse_fraction(s, _str, _pos));
    TRY(parse_exponent(s, _str, _pos));

    try {
      return JSONValue(std::stod(s));
    } catch (const std::out_of_range &e) {
      return Error("Invalid number format");
    }
  }

  auto Parser::parse_literal() -> ErrorOr<JSONValue>
  {
    if (_str.compare(_pos, sstr_runpack("true")) == 0) {
      _pos += lengthof("true");
      return JSONValue(true);
    }
    if (_str.compare(_pos, sstr_runpack("false")) == 0) {
      _pos += lengthof("false");
      return JSONValue(false);
    }
    if (_str.compare(_pos, sstr_runpack("null")) == 0) {
      _pos += lengthof("null");
      return JSONValue();
    }
    return Error("invalid litteral");
  }

  auto Parser::parse_array() -> ErrorOr<JSONArray>
  {
    JSONArray arr;

    MUST(_str[_pos++] == '[', "invalid object start");
    consume_whitespace();
    if (_str[_pos] == ']') {
      _pos++;
      return arr;
    }
    for (;;) {
      consume_whitespace();
      arr.push_back(TRY(eval()));
      consume_whitespace();
      if (_str[_pos] == ']')
        break;

      std::string err = "expected either comma or end of object, got [";
      err += _str[_pos];
      err += "]";

      MUST(_str[_pos++] == ',', err);
    }
    ++_pos;
    return arr;
  }

  auto Parser::parse_object() -> ErrorOr<JSONObject>
  {
    JSONObject obj;
    std::unordered_set<std::string> keys;

    MUST(_str[_pos++] == '{', "invalid object start");
    consume_whitespace();
    if (_str[_pos] == '}') {
      _pos++;
      return obj;
    }
    for (;;) {
      consume_whitespace();
      auto key = TRY(parse_string());
      if (keys.contains(key))
        return Error("duplicate key in object");
      keys.emplace(key);
      consume_whitespace();
      MUST(_str[_pos++] == ':', "expected colon in object");
      consume_whitespace();
      obj[std::string(key)] = TRY(eval());
      consume_whitespace();
      if (_str[_pos] == '}')
        break;

      std::string err = "expected either comma or end of object, got [";
      err += _str[_pos];
      err += "]";

      MUST(_str[_pos++] == ',', err);
    }
    ++_pos;
    return obj;
  }

  auto Parser::parse_string() -> ErrorOr<std::string>
  {
    std::string out;

    MUST(_str[_pos++] == '"', "incorred string start");

    for (; _pos < _str.size() && _str[_pos] != '"'; ++_pos)
      if (_str[_pos] == '\\')
        TRY(parse_escape(out, _str, _pos));
      else
        out += _str[_pos];
    MUST(_pos < _str.size(), "unterminated string");
    MUST(_str[_pos] == '"', "unterminated string");
    ++_pos;
    return out;
  }

}  // namespace JSON
