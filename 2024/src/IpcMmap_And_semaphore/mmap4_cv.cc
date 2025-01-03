#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <atomic>
#include <cstring>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <vector>
//基于条件变量的读写
const int NUM_SLOTS = 100;
const size_t MEMORY_SIZE = NUM_SLOTS * sizeof(int);

class SharedMemory {
public:
  SharedMemory() {
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
    if (munmap(memory_, MEMORY_SIZE) == -1) {
      std::cerr << "munmap failed: " << strerror(errno) << std::endl;
    }
  }

  void Write(int value) {
    std::unique_lock<std::mutex> lock(mutex_);
    // 等待直到有可用的写入位置
    data_available_.wait(lock, [this] {
        return (next_write_slot_.load() - next_read_slot_.load()) < NUM_SLOTS;
    });

    int slot = next_write_slot_.fetch_add(1, std::memory_order_relaxed) % NUM_SLOTS;
    memory_[slot] = value;
    std::cout << "Thread " << std::this_thread::get_id() << " wrote " << value
              << " to slot " << slot << std::endl;

    // 通知读者线程有数据可读
    data_ready_.notify_one();  // 只需要通知一个读者线程
  }

  void Read(int& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    // 等待直到有数据可读
    data_ready_.wait(lock, [this] { 
        return next_read_slot_.load() < next_write_slot_.load(); 
    });

    int slot = next_read_slot_.fetch_add(1, std::memory_order_relaxed) % NUM_SLOTS;
    value = memory_[slot];
    std::cout << "Thread " << std::this_thread::get_id() << " read " << value
              << " from slot " << slot << std::endl;

    // 通知所有写者线程有可用空间
    data_available_.notify_all(); 
  }

private:
  int* memory_;
  std::atomic<int> next_write_slot_;
  std::atomic<int> next_read_slot_;
  std::mutex mutex_;
  std::condition_variable data_ready_; 
  std::condition_variable data_available_; 
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
  for (int i = 0; i < 3; ++i) {  // 创建 3 个写者线程
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