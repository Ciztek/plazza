#ifndef JSON_HPP_
#define JSON_HPP_

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Parser {
  struct JSON;

  using JsonArray = std::vector<std::shared_ptr<JSON>>;
  using JsonObject = std::unordered_map<std::string, std::shared_ptr<JSON>>;

  struct JSON {
    using Value = std::
      variant<std::nullptr_t, bool, double, std::string, JsonArray, JsonObject>;

    Value value;

    JSON() : value(nullptr)
    {
    }

    JSON(std::nullptr_t) : value(nullptr)
    {
    }

    JSON(const std::string &str) : value(str)
    {
    }

    JSON(double num) : value(num)
    {
    }

    JSON(bool boolean) : value(boolean)
    {
    }

    JSON(const JsonArray &array) : value(array)
    {
    }

    JSON(const JsonObject &object) : value(object)
    {
    }

    auto operator[](const std::string &key) const -> const JSON &;
    auto operator[](size_t index) const -> const JSON &;
    template <typename T>
    auto static get_if_or_throw(
      const Parser::JSON &json,
      const std::string &context = "") -> const T *;

    JSON(const std::filesystem::path &path);
    [[nodiscard]] auto to_string(size_t depth = 0) const -> std::string;

  private:
    struct Parser {
      static auto string(const std::string &raw_data, size_t &pos) -> JSON;
      static auto number(const std::string &raw_data, size_t &pos) -> JSON;
      static auto literal(const std::string &raw_data, size_t &pos) -> JSON;
      static auto array(const std::string &raw_data, size_t &pos) -> JSON;
      static auto object(const std::string &raw_data, size_t &pos) -> JSON;
      static auto value(const std::string &raw_data, size_t &pos) -> JSON;
      static void consume_whitespace(const std::string &raw_data, size_t &pos);

      Parser() = delete;
      Parser(const Parser &) = delete;
      Parser(Parser &&) = delete;
      Parser &operator=(const Parser &) = delete;
      Parser &operator=(Parser &&) = delete;
      ~Parser() = delete;
    };
  };

  template <typename T>
  auto JSON::get_if_or_throw(const JSON &json, const std::string &context)
    -> const T *
  {
    if (auto *ptr = std::get_if<T>(&json.value))
      return ptr;
    throw std::runtime_error(
      "Invalid JSON format, expected " + std::string(typeid(T).name())
      + (context.empty() ? "" : " at '" + context + "'"));
  }
}  // namespace Parser

#endif /* !JSON_HPP_ */
