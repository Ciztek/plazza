/*
** EPITECH PROJECT, 2025
** plazza [WSL: Ubuntu]
** File description:
** SharedMemoryIpc
*/

#include "SharedMemoryIpc.hpp"

#include <stdexcept>

namespace shared_memory_ipc
{
    SharedMemoryIPC::SharedMemoryIPC(key_t key) : shm_key(key), shm_id(-1), shm_addr(nullptr), attached(false)
    {
        shm_id = shmget(shm_key, SEGMENT_SIZE, IPC_CREAT | 0666);
        if (shm_id == -1) {
            throw std::runtime_error("Failed to create shared memory segment");
        }
        shm_addr = static_cast<char *>(shmat(shm_id, nullptr, 0));
        if (shm_addr == reinterpret_cast<char *>(-1)) {
            throw std::runtime_error("Failed to attach shared memory segment");
        }
        attached = true;
    }

    SharedMemoryIPC::~SharedMemoryIPC()
    {
        if (attached) {
            shmdt(shm_addr);
        }
        shmctl(shm_id, IPC_RMID, nullptr);
    }

} // namespace shared_memory_ipc
