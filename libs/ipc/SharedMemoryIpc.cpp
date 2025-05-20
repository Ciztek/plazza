#include <stdexcept>

#include "ErrorOr.hpp"
#include "SharedMemoryIpc.hpp"

namespace IPC {
  SharedMemory::SharedMemory(key_t key)
    : shm_key(key), shm_id(-1), shm_addr(nullptr), attached(false)
  {
    shm_id = shmget(shm_key, SEGMENT_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1)
      throw std::runtime_error("Failed to create shared memory segment");
    shm_addr = static_cast<char *>(shmat(shm_id, nullptr, 0));
    if (
      shm_addr
      == reinterpret_cast<char *>(-1)) {  // NOLINT(performance-no-int-to-ptr)
      throw std::runtime_error("Failed to attach shared memory segment");
    }
    attached = true;
  }

  SharedMemory::~SharedMemory()
  {
    if (attached)
      shmdt(shm_addr);
    shmctl(shm_id, IPC_RMID, nullptr);
  }

  auto SharedMemory::write(const void *data, size_t size, size_t offset)
    -> MaybeError
  {
    if (offset + size > SEGMENT_SIZE)
      return Error("Write exceeds shared memory segment size");
    std::memcpy(shm_addr + offset, data, size);
    return Nil{};
  }

  auto SharedMemory::read(void *buffer, size_t size, size_t offset) const
    -> MaybeError
  {
    if (offset + size > SEGMENT_SIZE)
      return Error("Read exceeds shared memory segment size");
    std::memcpy(buffer, shm_addr + offset, size);
    return Nil{};
  }

  void SharedMemory::detach()
  {
    if (attached) {
      shmdt(shm_addr);
      attached = false;
    }
  }

  MaybeError SharedMemory::attach()
  {
    if (!attached) {
      shm_addr = static_cast<char *>(shmat(shm_id, nullptr, 0));
      if (
        shm_addr
        == reinterpret_cast<char *>(  // NOLINT(performance-no-int-to-ptr)
          -1)) {
        return Error("Failed to attach shared memory segment");
      }
      attached = true;
    }
    return Nil{};
  }

  void SharedMemory::remove()
  {
    if (attached)
      detach();
    shmctl(shm_id, IPC_RMID, nullptr);
  }

  auto SharedMemory::isValid() const -> bool
  {
    return shm_id != -1;
  }

  void SharedMemory::clear()
  {
    if (attached)
      std::memset(shm_addr, 0, SEGMENT_SIZE);
  }

  auto SharedMemory::getKey() const -> key_t
  {
    return shm_key;
  }

  auto SharedMemory::getId() const -> int
  {
    return shm_id;
  }

  template <typename T>
  auto SharedMemory::operator<<(const T &obj) -> SharedMemory &
  {
    write(&obj, sizeof(T));
    return *this;
  }

  template <typename T> auto SharedMemory::operator>>(T &obj) -> SharedMemory &
  {
    read(&obj, sizeof(T));
    return *this;
  }

}  // namespace IPC
