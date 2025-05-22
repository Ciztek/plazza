#pragma once

#include <stdexcept>
#include <unordered_map>
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
      _data[_used++] = value;
    }

    [[nodiscard]] auto size() const -> size_t
    {
      return _data.size();
    }

    [[nodiscard]] auto operator[](size_t index) const -> const T &
    {
      return _data[index];
    }

    [[nodiscard]] auto operator[](size_t index) -> T &
    {
      return _data[index];
    }

  private:
    std::vector<T> _data;
    size_t _used;
  };

  template <typename K, typename V> class BiMap {
  private:
    std::unordered_map<K, V> kv;
    std::unordered_map<V, K> vk;

  public:
    BiMap() = default;

    BiMap(size_t size) : kv(size), vk(size) {}

    BiMap(const BiMap &) = default;
    BiMap(BiMap &&) = default;
    auto operator=(const BiMap &) -> BiMap & = default;
    auto operator=(BiMap &&) -> BiMap & = default;

    ~BiMap() = default;

    [[nodiscard]] auto size() const -> size_t
    {
      return kv.size();
    }

    auto insert(const K &key, const V &value) -> MaybeError
    {
      if (kv.count(key) || vk.count(value))
        return Error("Duplicate key or value");
      kv[key] = value;
      vk[value] = key;
      return Nil{};
    }

    void remove_by_key(const K &key)
    {
      auto it = kv.find(key);
      if (it == kv.end())
        return;
      vk.erase(it->second);
      kv.erase(it);
    }

    void remove_by_value(const V &value)
    {
      auto it = vk.find(value);
      if (it == vk.end())
        return;
      kv.erase(it->second);
      vk.erase(it);
    }

    auto at_key(const K &key) const -> ErrorOr<V>
    {
      if (kv.count(key) == 0)
        return Error("Key not found");
      return kv.at(key);
    }

    auto at_value(const V &value) const -> ErrorOr<K>
    {
      if (vk.count(value) == 0)
        return Error("Key not found");
      return vk.at(value);
    }

    auto contains_key(const K &key) const -> bool
    {
      return kv.count(key) > 0;
    }

    auto contains_value(const V &value) const -> bool
    {
      return vk.count(value) > 0;
    }
  };

  using IdMapping = Data::BiMap<size_t, std::string>;
  using RecipeBook = std::
    unordered_map<size_t, std::pair<size_t, std::vector<size_t>>>;
}  // namespace Data
