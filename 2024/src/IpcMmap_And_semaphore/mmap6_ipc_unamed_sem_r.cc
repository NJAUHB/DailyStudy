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
*/
const int NUM_SLOTS = 100;
const size_t MEMORY_SIZE = sizeof(sem_t) * 2 + NUM_SLOTS * sizeof(int);  // 包含信号量空间
const char* SHARED_MEMORY_NAME = "/my_shared_memory6";

struct SharedData {
  sem_t sem_write;
  sem_t sem_read;
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

    next_write_slot_.store(0);
    next_read_slot_.store(0);
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

  void Read(int& value) {
    sem_wait(&shared_data_->sem_read);

    int slot = next_read_slot_.fetch_add(1, std::memory_order_relaxed) % NUM_SLOTS;
    value = shared_data_->data[slot];
    std::cout << "Reader Process: read " << value << " from slot " << slot << std::endl;

    sem_post(&shared_data_->sem_write);
  }

 private:
  SharedData* shared_data_;
  std::atomic<int> next_write_slot_;
  std::atomic<int> next_read_slot_;
  int shm_fd_;
};

void ReaderThread(SharedMemory& shared_memory) {
  for (int i = 0;; ++i) {
    int value;
    shared_memory.Read(value);
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}

int main() {
  SharedMemory shared_memory;

  std::thread reader(ReaderThread, std::ref(shared_memory));
  reader.join();

  return 0;
}