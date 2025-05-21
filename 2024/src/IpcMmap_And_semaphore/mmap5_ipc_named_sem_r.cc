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
Named semaphores 进程间通信
命名信号量用于进程间同步: 命名信号量存储在文件系统中，可以使用名称来访问，因此可以被不同进程的线程访问，从而实现进程间同步。
*/
const int NUM_SLOTS = 100;
const size_t MEMORY_SIZE = NUM_SLOTS * sizeof(int);
const char* SHARED_MEMORY_NAME = "/my_shared_memory";
const char* SEM_WRITE_NAME = "/my_sem_write";
const char* SEM_READ_NAME = "/my_sem_read";

class SharedMemory {
 public:
  SharedMemory() {
    // 打开共享内存对象
    shm_fd_ = shm_open(SHARED_MEMORY_NAME, O_RDWR, S_IRUSR | S_IWUSR);  //  Named semaphores
    if (shm_fd_ == -1) {
      std::cerr << "shm_open failed: " << strerror(errno) << std::endl;
      exit(1);
    }

    // 映射共享内存到进程地址空间
    memory_ = static_cast<int*>(mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0));
    if (memory_ == MAP_FAILED) {
      std::cerr << "mmap failed: " << strerror(errno) << std::endl;
      exit(1);
    }

    // 打开信号量
    sem_write_ = sem_open(SEM_WRITE_NAME, 0);
    if (sem_write_ == SEM_FAILED) {
      std::cerr << "sem_open failed for sem_write_: " << strerror(errno) << std::endl;
      exit(1);
    }
    sem_read_ = sem_open(SEM_READ_NAME, 0);
    if (sem_read_ == SEM_FAILED) {
      std::cerr << "sem_open failed for sem_read_: " << strerror(errno) << std::endl;
      exit(1);
    }

    next_write_slot_.store(0);
    next_read_slot_.store(0);
  }

  ~SharedMemory() {
    if (munmap(memory_, MEMORY_SIZE) == -1) {
      std::cerr << "munmap failed: " << strerror(errno) << std::endl;
    }
    if (close(shm_fd_) == -1) {
      std::cerr << "close failed: " << strerror(errno) << std::endl;
    }
    if (sem_close(sem_write_) == -1) {
      std::cerr << "sem_close failed for sem_write_: " << strerror(errno) << std::endl;
    }
    if (sem_close(sem_read_) == -1) {
      std::cerr << "sem_close failed for sem_read_: " << strerror(errno) << std::endl;
    }
  }

  void Read(int& value) {
    // 等待信号量，表示有数据可读
    sem_wait(sem_read_);

    int slot = next_read_slot_.fetch_add(1, std::memory_order_relaxed) % NUM_SLOTS;
    value = memory_[slot];
    std::cout << "Reader Process: read " << value << " from slot " << slot << std::endl;

    // 通知写者有可用空间
    sem_post(sem_write_);
  }

 private:
  int* memory_;
  std::atomic<int> next_write_slot_;
  std::atomic<int> next_read_slot_;
  int shm_fd_;
  sem_t* sem_write_;
  sem_t* sem_read_;
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