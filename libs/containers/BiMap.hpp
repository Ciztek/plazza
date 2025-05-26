#include <cstddef>
#include <unordered_map>

#include "ErrorOr.hpp"

template <typename K, typename V> class BiMap {
private:
  std::unordered_map<K, V> kv;
  std::unordered_map<V, K> vk;

public:
  BiMap() = default;

  explicit BiMap(size_t size) : kv(size), vk(size) {}

  BiMap(const BiMap &) = default;
  BiMap(BiMap &&) noexcept = default;
  auto operator=(const BiMap &) -> BiMap & = default;
  auto operator=(BiMap &&) noexcept -> BiMap & = default;

  ~BiMap() = default;

  [[nodiscard]] auto size() const noexcept -> size_t
  {
    return kv.size();
  }

  [[nodiscard]] auto empty() const noexcept -> bool
  {
    return kv.empty();
  }

  void clear() noexcept
  {
    kv.clear();
    vk.clear();
  }

  auto insert(const K &key, const V &value) -> MaybeError
  {
    if (kv.contains(key) || vk.contains(value))
      return Error("Duplicate key or value");
    kv[key] = value;
    vk[value] = key;
    return Nil{};
  }

  void remove_by_key(const K &key) noexcept
  {
    auto it = kv.find(key);
    if (it == kv.end())
      return;
    vk.erase(it->second);
    kv.erase(it);
  }

  void remove_by_value(const V &value) noexcept
  {
    auto it = vk.find(value);
    if (it == vk.end())
      return;
    kv.erase(it->second);
    vk.erase(it);
  }

  auto at_key(const K &key) const -> ErrorOr<V>
  {
    auto it = kv.find(key);
    if (it == kv.end())
      return Error("Key not found");
    return it->second;
  }

  auto at_value(const V &value) const -> ErrorOr<K>
  {
    auto it = vk.find(value);
    if (it == vk.end())
      return Error("Value not found");
    return it->second;
  }

  [[nodiscard]] auto contains_key(const K &key) const noexcept -> bool
  {
    return kv.contains(key);
  }

  [[nodiscard]] auto contains_value(const V &value) const noexcept -> bool
  {
    return vk.contains(value);
  }
};
