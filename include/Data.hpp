#pragma once

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "ErrorOr.hpp"

namespace Data {
  template <typename T> class FixedArray {
  public:
    FixedArray() = default;

    void setSize(size_t size)
    {
      if (!_data.empty())
        throw std::runtime_error("Array already initialized");
      _data.resize(size);
      _used = 0;
    }

    // Use _used to know how many are currently used
    [[nodiscard]] auto used() const -> size_t
    {
      if (_data.empty())
        throw std::runtime_error("Array not initialized");
      return _used;
    }

    void add(const T &value)
    {
      if (_data.empty())
        throw std::runtime_error("Array not initialized");
      if (_used >= _data.size())
        throw std::runtime_error("Array is full");
      _data[_used++] = value;
    }

    [[nodiscard]] auto size() const -> size_t
    {
      if (_data.empty())
        throw std::runtime_error("Array not initialized");
      return _data.size();
    }

    [[nodiscard]] auto operator[](size_t index) const -> const T &
    {
      if (_data.empty())
        throw std::runtime_error("Array not initialized");
      if (index >= _used)
        throw std::out_of_range("Index out of used range");
      return _data[index];
    }

    [[nodiscard]] auto operator[](size_t index) -> T &
    {
      if (_data.empty())
        throw std::runtime_error("Array not initialized");
      if (index >= _used)
        throw std::out_of_range("Index out of used range");
      return _data[index];
    }

  private:
    std::vector<T> _data;
    size_t _used = 0;
  };

  class Ids : public FixedArray<std::string> {
  public:
    Ids() = default;

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
      for (size_t i = 0; i < ids.size(); ++i)
        os << "  " << i << ": " << ids.lookup(i)
           << (i != ids.size() - 1 ? "," : "") << "\n";
      os << "]";
      return os;
    }
  };

  using RecipeBook = std::map<size_t, std::pair<size_t, std::vector<size_t>>>;

}  // namespace Data
