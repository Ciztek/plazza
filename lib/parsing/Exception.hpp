#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include <string>
#include <utility>

namespace Parser {

  class FileException : public std::exception {
  public:
    explicit FileException(std::string message) : msg_(std::move(message))
    {
    }

    [[nodiscard]] virtual const char *what() const noexcept override
    {
      return msg_.c_str();
    }

  private:
    std::string msg_;
  };

  class ParserException : public std::exception {
  public:
    explicit ParserException(std::string message) : msg_(std::move(message))
    {
    }

    [[nodiscard]] virtual const char *what() const noexcept override
    {
      return msg_.c_str();
    }

  private:
    std::string msg_;
  };
}  // namespace Parser

#endif /* !EXCEPTION_HPP_ */
