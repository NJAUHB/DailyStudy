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

const int NUM_SLOTS = 50;
const size_t MEMORY_SIZE = sizeof(sem_t) * 2 + sizeof(std::atomic<int>) * 3 + NUM_SLOTS * sizeof(int); 
const char* SHARED_MEMORY_NAME = "/my_shared_memoryw7";

enum RWStatus {
  IDLE = 0,
  WRITING,
  READING
};

struct SharedData {
  sem_t sem_write;
  sem_t sem_read;
  std::atomic<int> rw_status;
  std::atomic<int> next_write_slot_; 
  std::atomic<int> next_read_slot_; 
  int data[NUM_SLOTS];
};

class SharedMemory {
public:
  SharedMemory() {
    // 创建或打开共享内存对象
    shm_fd_ = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); // shm_buf_open
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
    shared_data_ = static_cast<SharedData*>(mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE,
                                        MAP_SHARED, shm_fd_, 0));
    if (shared_data_ == MAP_FAILED) {
      std::cerr << "mmap failed: " << strerror(errno) << std::endl;
      exit(1);
    }

    shared_data_->next_write_slot_.store(0);
    shared_data_->next_read_slot_.store(0);
    // 初始化信号量 (只在创建共享内存的进程中执行一次)
    if (shared_data_->next_write_slot_.load() == 0) { 
      std::cout << "sem init" << std::endl;
      sem_init(&shared_data_->sem_write, 1, NUM_SLOTS);
      sem_init(&shared_data_->sem_read, 1, 0);
      shared_data_->rw_status.store(IDLE); // 初始化读写状态为空闲
    }

   
  }

  ~SharedMemory() {
    int end_flag = 999;
    Write(end_flag);
    std::cout << " sen end flag. " << std::endl;
    if (munmap(shared_data_, MEMORY_SIZE) == -1) {
      std::cerr << "munmap failed: " << strerror(errno) << std::endl;
    }
    if (close(shm_fd_) == -1) {
      std::cerr << "close failed: " << strerror(errno) << std::endl;
    }
    // 不需要在析构函数中销毁信号量，因为其他进程可能还在使用
  }

  void Write(int value) {
    while ((shared_data_->next_write_slot_.load() - shared_data_->next_read_slot_.load() + NUM_SLOTS) % NUM_SLOTS == 0) {
      int expected_status = expected_status_.load();
      if (shared_data_->rw_status.compare_exchange_strong(expected_status, WRITING)) {
        break;  // 获取写锁成功，跳出循环
      }
    }

    int slot = shared_data_->next_write_slot_.fetch_add(1, std::memory_order_acq_rel) % NUM_SLOTS;
    shared_data_->data[slot] = value;
    std::cout << "Writer Process: Thread " << std::this_thread::get_id()
              << " wrote " << value << " to slot " << slot << std::endl;
    std::cout << "next_read_slot_ : " <<  shared_data_->next_read_slot_.load() << std::endl;

    sem_post(&shared_data_->sem_read);
    // 释放读锁
    shared_data_->rw_status.store(IDLE);
  }

private:
  SharedData* shared_data_;
  // std::atomic<int> next_write_slot_; // 这里有问题，作为私有变量，w进程无法感知到r进程的next_read_slot_
  // std::atomic<int> next_read_slot_;
  int shm_fd_;
  std::atomic<int> expected_status_{IDLE}; // 预期的读写状态，初始为空闲
};

void WriterThread(SharedMemory* shared_memory) {
  for (int i = 0; i < 100; ++i) {
    shared_memory->Write(i);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
  }
}

int main() {
  std::shared_ptr<SharedMemory> shared_memory_;
  shared_memory_ = std::make_shared<SharedMemory>();

  std::vector<std::thread> writers;
  for (int i = 0; i < 3; ++i) {
    writers.emplace_back(WriterThread, shared_memory_.get());
  }

  for (auto& writer : writers) {
    writer.join();
  }

  return 0;
}