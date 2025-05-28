#pragma once

#include <cctype>
#include <string>
#include <vector>

#include "ErrorOr.hpp"

struct Order {
  std::string type;
  std::string size;
  int number;
};

enum class TokenType : std::uint8_t {
  Word,
  Size,
  Number,
  Semicolon,
  End
};

struct Token {
  TokenType type;
  std::string value;
};

class Lexer {
public:
  explicit Lexer(std::string input);

  auto tokenize() -> ErrorOr<std::vector<Token>>;

private:
  std::string _input;
  size_t _pos;

  void skip_whitespace();

  auto consume_word() -> std::string;

  auto consume_number() -> ErrorOr<std::string>;

  static auto is_valid_size(const std::string &s) -> bool;
};

class Parser {
public:
  explicit Parser(const std::vector<Token> &tokens);

  auto parse() -> ErrorOr<std::vector<Order>>;

private:
  const std::vector<Token> &_tokens;
  size_t _pos;

  auto parse_order() -> ErrorOr<Order>;

  auto expect(TokenType expected) -> ErrorOr<Token>;

  auto match(TokenType type) -> bool;
};
