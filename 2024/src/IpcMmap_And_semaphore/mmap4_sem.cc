#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <atomic>
#include <cstring>
#include <thread>
#include <chrono>
#include <semaphore.h>
#include <vector>
/*
Unnamed semaphores (memory-based semaphores) 同进程通信

非命名信号量用于线程同步: 非命名信号量通常用于同一个进程内不同线程之间的同步。它们存储在进程的内存空间中，只能被该进程内的线程访问
*/
const int NUM_SLOTS = 100;
const size_t MEMORY_SIZE = NUM_SLOTS * sizeof(int);

class SharedMemory {
public:
  SharedMemory() {
    // 初始化信号量
    sem_init(&sem_write_, 0, NUM_SLOTS); // 初始化为 NUM_SLOTS，表示初始有 NUM_SLOTS 个可用空间
    sem_init(&sem_read_, 0, 0);           // 初始化为 0，表示初始没有数据可读

    memory_ = static_cast<int*>(mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE,
                                     MAP_SHARED | MAP_ANONYMOUS, -1, 0));
    if (memory_ == MAP_FAILED) {
      std::cerr << "mmap failed: " << strerror(errno) << std::endl;
      exit(1);
    }
    std::memset(memory_, 0, MEMORY_SIZE);
    next_write_slot_.store(0);
    next_read_slot_.store(0);
  }

  ~SharedMemory() {
    // 销毁信号量
    sem_destroy(&sem_write_);
    sem_destroy(&sem_read_);

    if (munmap(memory_, MEMORY_SIZE) == -1) {
      std::cerr << "munmap failed: " << strerror(errno) << std::endl;
    }
  }

  void Write(int value) {
    // 等待信号量，表示有可用空间
    sem_wait(&sem_write_);

    int slot = next_write_slot_.fetch_add(1, std::memory_order_relaxed) % NUM_SLOTS;
    memory_[slot] = value;
    std::cout << "Thread " << std::this_thread::get_id() << " wrote " << value
              << " to slot " << slot << std::endl;

    // 通知读者线程有数据可读
    sem_post(&sem_read_); 
  }

  void Read(int& value) {
    // 等待信号量，表示有数据可读
    sem_wait(&sem_read_);

    int slot = next_read_slot_.fetch_add(1, std::memory_order_relaxed) % NUM_SLOTS;
    value = memory_[slot];
    std::cout << "Thread " << std::this_thread::get_id() << " read " << value
              << " from slot " << slot << std::endl;

    // 通知写者线程有可用空间
    sem_post(&sem_write_);
  }

private:
  int* memory_;
  std::atomic<int> next_write_slot_;
  std::atomic<int> next_read_slot_;
  sem_t sem_write_;
  sem_t sem_read_;
};

void WriterThread(SharedMemory& shared_memory) {
  for (int i = 0; ; ++i) { 
    shared_memory.Write(i * 10);
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
  }
}

void ReaderThread(SharedMemory& shared_memory) {
  for (int i = 0; ; ++i) {
    int value;
    shared_memory.Read(value);
    std::this_thread::sleep_for(std::chrono::milliseconds(15)); 
  }
}

int main() {
  SharedMemory shared_memory;

  // 创建多个写者线程
  std::vector<std::thread> writers;
  for (int i = 0; i < 3; ++i) { 
    writers.emplace_back(WriterThread, std::ref(shared_memory));
  }

  // 创建一个读者线程
  std::thread reader(ReaderThread, std::ref(shared_memory));

  // 等待所有线程结束
  for (auto& writer : writers) {
    writer.join();
  }
  reader.join();

  return 0;
}