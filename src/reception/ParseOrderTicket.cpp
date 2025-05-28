#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include "ErrorOr.hpp"
#include "TicketFacade.hpp"
#include "TicketInternal.hpp"
#include "logging/Logger.hpp"

Lexer::Lexer(std::string input) : _input(std::move(input)), _pos(0) {}

auto Lexer::tokenize() -> ErrorOr<std::vector<Token>>
{
  std::vector<Token> tokens;

  while (_pos < _input.size()) {
    skip_whitespace();
    if (_pos >= _input.size())
      break;

    if (_input[_pos] == ';') {
      tokens.push_back({TokenType::Semicolon, ";"});
      ++_pos;
      continue;
    }

    if (!std::isalpha(_input[_pos])) {
      return Error(
        "Unexpected character: '" + std::string(1, _input[_pos]) + "'");
    }

    // TODO: improve detection with further lookahead
    skip_whitespace();
    if (_input[_pos] == 'x') {
      auto num = TRY(consume_number());
      tokens.push_back({TokenType::Number, num});
    }
    skip_whitespace();
    std::string word = consume_word();
    if (is_valid_size(word))
      tokens.push_back({TokenType::Size, word});
    else if (!std::ranges::all_of(word, isspace))
      tokens.push_back({TokenType::Word, word});
  }

  tokens.push_back({TokenType::End, ""});
  return tokens;
}

void Lexer::skip_whitespace()
{
  while (_pos < _input.size() && std::isspace(_input[_pos]))
    ++_pos;
}

auto Lexer::consume_word() -> std::string
{
  size_t start = _pos;

  Log::debug << "Comsuming a word";
  while (_pos < _input.size() && std::isalpha(_input[_pos]))
    ++_pos;
  return _input.substr(start, _pos - start);
}

auto Lexer::consume_number() -> ErrorOr<std::string>
{
  size_t start = _pos;

  Log::debug << "Comsuming a number";
  if (_input[_pos++] != 'x')
    return Error("Expected 'x' prefix for number");

  if (_pos >= _input.size() || _input[_pos] == '0')
    return Error("Expected non-zero digit after 'x'");

  while (_pos < _input.size() && std::isdigit(_input[_pos]))
    ++_pos;

  return _input.substr(start, _pos - start);
}

auto Lexer::is_valid_size(const std::string &s) -> bool
{
  return s == "S" || s == "M" || s == "L" || s == "XL" || s == "XXL";
}

Parser::Parser(const std::vector<Token> &tokens) : _tokens(tokens), _pos(0) {}

auto Parser::parse() -> ErrorOr<std::vector<Order>>
{
  std::vector<Order> orders;

  while (!match(TokenType::End)) {
    Log::debug << "Parsing new order";
    auto order = TRY(parse_order());
    orders.push_back(order);

    if (!match(TokenType::End))
      TRY(expect(TokenType::Semicolon));
  }
  return orders;
}

auto Parser::parse_order() -> ErrorOr<Order>
{
  auto type = TRY(expect(TokenType::Word)).value;
  auto size = TRY(expect(TokenType::Size)).value;
  auto number_str = TRY(expect(TokenType::Number)).value;

  int count = std::stoi(number_str.substr(1));
  return Order{.type = type, .size = size, .number = count};
}

auto Parser::expect(TokenType expected) -> ErrorOr<Token>
{
  Log::debug << "expecting!";
  if (_pos >= _tokens.size() || _tokens[_pos].type != expected)
    return Error("Unexpected token: '" + _tokens[_pos].value + "'");
  return _tokens[_pos++];
}

auto Parser::match(TokenType type) -> bool
{
  return _pos < _tokens.size() && _tokens[_pos].type == type;
}

auto take_order_ticket(const std::string &str) -> ErrorOr<std::vector<Order>>
{
  Lexer lexer(str);
  auto tokens = TRY(lexer.tokenize());

  Parser parser(tokens);
  return parser.parse();
}
