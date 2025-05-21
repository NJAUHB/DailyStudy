#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <atomic>
#include <cstring>
#include <thread>
#include <chrono>
#include <semaphore.h>
/*
Unamed semaphores 进程间通信
匿名信号量用于进程间同步: 基于共享内存
覆盖多写和读：需要解决写和读竞争的问题
*/
const int NUM_SLOTS = 50;
const size_t MEMORY_SIZE = sizeof(sem_t) * 2 + sizeof(std::atomic<int>) * 3 + NUM_SLOTS * sizeof(int);
const char* SHARED_MEMORY_NAME = "/my_shared_memoryw7";

enum RWStatus { IDLE = 0, WRITING, READING };

struct SharedData {
  sem_t sem_write;
  sem_t sem_read;
  std::atomic<int> rw_status;  // 读写状态
  std::atomic<int> next_write_slot_;
  std::atomic<int> next_read_slot_;
  int data[NUM_SLOTS];
};

class SharedMemory {
 public:
  SharedMemory() {
    // 打开共享内存对象
    shm_fd_ = shm_open(SHARED_MEMORY_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd_ == -1) {
      std::cerr << "shm_open failed: " << strerror(errno) << std::endl;
      exit(1);
    }

    // 映射共享内存到进程地址空间
    shared_data_ = static_cast<SharedData*>(mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0));
    if (shared_data_ == MAP_FAILED) {
      std::cerr << "mmap failed: " << strerror(errno) << std::endl;
      exit(1);
    }

    // shared_data_->next_write_slot_.store(0);
    // shared_data_->next_read_slot_.store(0);
  }

  ~SharedMemory() {
    if (munmap(shared_data_, MEMORY_SIZE) == -1) {
      std::cerr << "munmap failed: " << strerror(errno) << std::endl;
    }
    if (close(shm_fd_) == -1) {
      std::cerr << "close failed: " << strerror(errno) << std::endl;
    }

    // 在最后一个使用共享内存的进程 (reader) 中销毁信号量
    if (shm_unlink(SHARED_MEMORY_NAME) == -1) {
      std::cerr << "shm_unlink failed: " << strerror(errno) << std::endl;
    }
    sem_destroy(&shared_data_->sem_write);
    sem_destroy(&shared_data_->sem_read);
  }

  void Read(int& value, bool& should_continue) {
    sem_wait(&shared_data_->sem_read);
    while ((shared_data_->next_write_slot_.load() - shared_data_->next_read_slot_.load() + NUM_SLOTS) % NUM_SLOTS ==
           0) {
      int expected_status = expected_status_.load();
      if (shared_data_->rw_status.compare_exchange_strong(expected_status, READING)) {
        break;  // 获取写锁成功，跳出循环
      }
    }

    int slot = shared_data_->next_read_slot_.fetch_add(1, std::memory_order_relaxed) % NUM_SLOTS;
    value = shared_data_->data[slot];
    std::cout << "Reader Process: read " << value << " from slot " << slot << std::endl;
    if (value == 999) {
      std::cout << "READ END FLAG" << std::endl;
      should_continue = false;
    }
    // 释放读锁
    shared_data_->rw_status.store(IDLE);
  }

 private:
  SharedData* shared_data_;
  int shm_fd_;
  std::atomic<int> expected_status_ {IDLE};  // 预期的读写状态，初始为空闲
};

void ReaderThread(SharedMemory& shared_memory) {
  bool should_continue = true;
  for (int i = 0; should_continue; ++i) {
    int value;
    shared_memory.Read(value, should_continue);
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}

int main() {
  SharedMemory shared_memory;

  std::thread reader(ReaderThread, std::ref(shared_memory));
  reader.join();

  return 0;
}