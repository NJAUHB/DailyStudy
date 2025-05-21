#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <atomic>
#include <cstring>
#include <thread>
#include <chrono>
#include <vector>
#include <semaphore.h>

const int NUM_SLOTS = 100;
const size_t MEMORY_SIZE = NUM_SLOTS * sizeof(int);
const char* SHARED_MEMORY_NAME = "/my_shared_memory";
const char* SEM_WRITE_NAME = "/my_sem_write";
const char* SEM_READ_NAME = "/my_sem_read";

class SharedMemory {
 public:
  SharedMemory() {
    // 创建或打开共享内存对象
    shm_fd_ = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd_ == -1) {
      std::cerr << "shm_open failed: " << strerror(errno) << std::endl;
      exit(1);
    }

    // 设置共享内存大小
    if (ftruncate(shm_fd_, MEMORY_SIZE) == -1) {
      std::cerr << "ftruncate failed: " << strerror(errno) << std::endl;
      exit(1);
    }

    // 映射共享内存到进程地址空间
    memory_ = static_cast<int*>(mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0));
    if (memory_ == MAP_FAILED) {
      std::cerr << "mmap failed: " << strerror(errno) << std::endl;
      exit(1);
    }

    // 创建或打开信号量
    sem_write_ = sem_open(SEM_WRITE_NAME, O_CREAT, S_IRUSR | S_IWUSR, NUM_SLOTS);
    if (sem_write_ == SEM_FAILED) {
      std::cerr << "sem_open failed for sem_write_: " << strerror(errno) << std::endl;
      exit(1);
    }
    sem_read_ = sem_open(SEM_READ_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (sem_read_ == SEM_FAILED) {
      std::cerr << "sem_open failed for sem_read_: " << strerror(errno) << std::endl;
      exit(1);
    }

    std::memset(memory_, 0, MEMORY_SIZE);
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

  void Write(int value) {
    // 等待信号量，表示有可用空间
    sem_wait(sem_write_);  // 这一行如果注释掉，就会一直覆盖写（无锁）

    int slot = next_write_slot_.fetch_add(1, std::memory_order_relaxed) % NUM_SLOTS;
    memory_[slot] = value;
    std::cout << "Writer Process: Thread " << std::this_thread::get_id() << " wrote " << value << " to slot " << slot
              << std::endl;

    // 通知读者有数据可读
    sem_post(sem_read_);
  }

 private:
  int* memory_;
  std::atomic<int> next_write_slot_;
  std::atomic<int> next_read_slot_;
  int shm_fd_;
  sem_t* sem_write_;
  sem_t* sem_read_;
};

void WriterThread(SharedMemory& shared_memory) {
  for (int i = 0;; ++i) {
    shared_memory.Write(i * 10);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int main() {
  SharedMemory shared_memory;

  // 创建多个写者线程
  std::vector<std::thread> writers;
  for (int i = 0; i < 3; ++i) {
    writers.emplace_back(WriterThread, std::ref(shared_memory));
  }

  // 等待所有线程结束
  for (auto& writer : writers) {
    writer.join();
  }

  return 0;
}