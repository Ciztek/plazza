/*
** EPITECH PROJECT, 2025
** plazza
** File description:
** SharedMemoryIpc
*/

#ifndef SHARED_MEMORY_IPC_HPP_
    #define SHARED_MEMORY_IPC_HPP_

    #include <cstring>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <sys/types.h>
    #include <unistd.h>

    namespace shared_memory_ipc
    {
        class SharedMemoryIPC
        {
            public:
                static constexpr size_t SEGMENT_SIZE = 4096;

                SharedMemoryIPC(key_t key);
                ~SharedMemoryIPC();

                void write(const void *data, size_t size, size_t offset = 0);
                void read(void *buffer, size_t size, size_t offset = 0) const;

                void detach();
                void attach();
                void remove();
                [[nodiscard]] auto isValid() const -> bool;
                void clear();

                [[nodiscard]] auto getKey() const -> key_t;
                [[nodiscard]] auto getId() const -> int;

                // Operator overloads for serialization
                template<typename T>
                auto operator<<(const T &obj) -> SharedMemoryIPC &;

                template<typename T>
                auto operator>>(T &obj) -> SharedMemoryIPC &;

            private:
                key_t shm_key;
                int shm_id;
                char *shm_addr;
                bool attached;
        };
    } // namespace shared_memory_ipc

#endif /* !SHARED_MEMORY_IPC_HPP_ */
