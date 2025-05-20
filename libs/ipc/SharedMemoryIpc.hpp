#pragma once

#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include "ErrorOr.hpp"

namespace IPC {
  class SharedMemory {
  public:
    static constexpr size_t SEGMENT_SIZE = 4096;

    SharedMemory(key_t key);
    ~SharedMemory();

    auto write(const void *data, size_t size, size_t offset = 0) -> MaybeError;
    auto
    read(void *buffer, size_t size, size_t offset = 0) const -> MaybeError;

    void detach();
    auto attach() -> MaybeError;
    void remove();
    [[nodiscard]] auto isValid() const -> bool;
    void clear();

    [[nodiscard]] auto getKey() const -> key_t;
    [[nodiscard]] auto getId() const -> int;

    // Operator overloads for serialization
    template <typename T> auto operator<<(const T &obj) -> SharedMemory &;

    template <typename T> auto operator>>(T &obj) -> SharedMemory &;

  private:
    key_t shm_key;
    int shm_id;
    char *shm_addr;
    bool attached;
  };
}  // namespace IPC
