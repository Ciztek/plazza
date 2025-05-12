#pragma once

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

namespace Data {
  template <typename T> class FixedArray {
  public:
    FixedArray() = default;

    void setSize(size_t size)
    {
      if (!_data.empty())
        throw std::runtime_error("Array already initialized");
      _data.resize(size);
    }

    FixedArray(const FixedArray &) = default;
    FixedArray(FixedArray &&) = default;
    auto operator=(const FixedArray &) -> FixedArray & = default;
    auto operator=(FixedArray &&) -> FixedArray & = default;

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
      if (index >= _data.size())
        throw std::out_of_range("Index out of range");
      return _data[index];
    }

    [[nodiscard]] auto operator[](size_t index) -> T &
    {
      if (_data.empty())
        throw std::runtime_error("Array not initialized");
      if (index >= _data.size())
        throw std::out_of_range("Index out of range");
      return _data[index];
    }

  private:
    std::vector<T> _data;
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

    [[nodiscard]] auto lookup(const std::string &value) const -> size_t
    {
      for (size_t i = 0; i < this->size(); ++i)
        if (this->operator[](i) == value)
          return i;
      throw std::runtime_error("Not found");
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

  using Recipe = std::pair<size_t, std::vector<size_t>>;
  using RecipeBook = std::map<size_t, Recipe>;

}  // namespace Data
