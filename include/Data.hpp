#pragma once

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "ErrorOr.hpp"

namespace Data {
  template <typename T> class FixedArray {
  public:
    explicit FixedArray(size_t size) : _data(size), _used(0)
    {
      if (size == 0)
        throw std::runtime_error("FixedArray size cannot be 0");
    }

    [[nodiscard]] auto used() const -> size_t
    {
      return _used;
    }

    void add(const T &value)
    {
      if (_used >= _data.size())
        throw std::runtime_error("Array is full");
      _data[_used++] = value;
    }

    [[nodiscard]] auto size() const -> size_t
    {
      return _data.size();
    }

    [[nodiscard]] auto operator[](size_t index) const -> const T &
    {
      if (index >= _used)
        throw std::out_of_range("Index out of used range");
      return _data[index];
    }

    [[nodiscard]] auto operator[](size_t index) -> T &
    {
      if (index >= _used)
        throw std::out_of_range("Index out of used range");
      return _data[index];
    }

  private:
    std::vector<T> _data;
    size_t _used;
  };

  class Ids : public FixedArray<std::string> {
  public:
    explicit Ids(size_t size) : FixedArray(size) {}

    Ids(const Ids &) = delete;
    Ids(Ids &&) = delete;
    auto operator=(const Ids &) -> Ids & = delete;
    auto operator=(Ids &&) -> Ids & = delete;

    [[nodiscard]] auto lookup(size_t index) const -> const std::string &
    {
      return this->operator[](index);
    }

    [[nodiscard]] auto
    lookup(const std::string &value) const -> ErrorOr<size_t>
    {
      for (size_t i = 0; i < this->used(); ++i)
        if (this->operator[](i) == value)
          return i;
      return Error("Not found");
    }

    void add(const std::string &value)
    {
      for (size_t i = 0; i < this->used(); ++i)
        if (this->operator[](i) == value)
          return;
      this->FixedArray<std::string>::add(value);
    }

    friend auto operator<<(std::ostream &os, const Ids &ids) -> std::ostream &
    {
      os << "[\n";
      for (size_t i = 0; i < ids.used(); ++i)
        os << "  " << i << ": " << ids.lookup(i)
           << (i != ids.used() - 1 ? "," : "") << "\n";
      os << "]";
      return os;
    }
  };

  using RecipeBook = std::map<size_t, std::pair<size_t, std::vector<size_t>>>;

  template <typename T> class Singleton {
  public:
    static auto instance() -> T &
    {
      static T instance;
      return instance;
    }

    // Delete copy and move constructors and assign operators
    Singleton(Singleton const &) = delete;
    Singleton(Singleton &&) = delete;
    auto operator=(Singleton const &) -> Singleton & = delete;
    auto operator=(Singleton &&) -> Singleton & = delete;

  protected:
    Singleton() = default;
    virtual ~Singleton() = default;
  };
}  // namespace Data
