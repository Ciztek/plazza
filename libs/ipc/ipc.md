# SharedMemoryIPC: Design & Usage

## Overview

The `SharedMemoryIPC` class encapsulates a shared memory segment for Inter-Process Communication (IPC) between processes, such as the Reception and Kitchen processes in your Plazza project. It provides a simple, type-safe interface for reading and writing data, including serialization support for custom objects (like pizza orders).

---

## Key Features

- **Fixed Segment Size:**
  The shared memory segment is always 4096 bytes (`SEGMENT_SIZE`).

- **Ownership & Lifetime:**
  The segment is created/attached in the constructor and cleaned up in the destructor. You can also manually attach, detach, or remove the segment.

- **Read/Write Operations:**
  - `write(const void *data, size_t size, size_t offset = 0)`: Write raw data to the segment at a given offset.
  - `read(void *buffer, size_t size, size_t offset = 0) const`: Read raw data from the segment at a given offset.

- **Serialization Operators:**
  - `operator<<` and `operator>>` allow you to serialize/deserialize objects (e.g., pizza orders) directly to/from shared memory, provided your objects implement `pack` and `unpack` methods.

- **Utility Methods:**
  - `detach()`, `attach()`, `remove()`: Manage the shared memory segment's lifecycle.
  - `isValid()`: Check if the segment is currently attached and usable.
  - `clear()`: Zero out the shared memory segment.
  - `getKey()`, `getId()`: Retrieve the segment's key and ID for debugging or management.

---

## Example Usage

### Writing a Pizza Order

```cpp
PizzaOrder order = ...; // Your serializable pizza order object
SharedMemoryIPC shm(key);
shm << order; // Serializes and writes the order to shared memory
```

### Reading a Pizza Order

```cpp
PizzaOrder order;
SharedMemoryIPC shm(key);
shm >> order; // Reads and deserializes the order from shared memory
```

### Raw Data

```cpp
char buffer[100];
shm.write(buffer, sizeof(buffer));
shm.read(buffer, sizeof(buffer));
```

---

## Serialization Requirements

For `operator<<` and `operator>>` to work, your object must implement:

```cpp
void pack(char *dest, size_t maxSize) const;
void unpack(const char *src, size_t maxSize);
```

---

## Thread Safety

**Note:**
Shared memory itself is not thread-safe. If multiple processes/threads access the segment concurrently, you must implement your own synchronization (e.g., using semaphores or mutexes).

---

## Typical Workflow

1. **Reception** creates or attaches to a shared memory segment.
2. **Reception** serializes and writes a pizza order.
3. **Kitchen** attaches to the same segment, reads, and deserializes the order.
4. Both processes can detach or remove the segment as needed.

---

## Error Handling

- If an operation fails (e.g., attach, read, write), the class should throw an exception or set an error state.
- Always check `isValid()` before performing operations.
