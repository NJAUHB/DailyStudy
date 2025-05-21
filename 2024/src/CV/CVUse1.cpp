#include <chrono>
#include <mutex>
#include <condition_variable>

const auto TimeoutDuration = std::chrono::seconds(10);
void TimerThread(std::atomic_bool& testCompleted, std::atomic_bool& timedOut, std::condition_variable& cv,
                 std::mutex& cv_mutex) {
  std::unique_lock<std::mutex> lock(cv_mutex);
  if (!cv.wait_for(lock, TimeoutDuration, [&testCompleted] { return testCompleted.load(); })) {
    // time out
    timedOut.store(true);
  }
}
