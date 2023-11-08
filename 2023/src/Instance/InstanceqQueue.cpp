#include <iostream>
#include <queue>
#include <mutex>

class QueueSingleton {
private:
   std::queue<int> queue;
   static std::mutex mtx;

   // 私有构造函数，避免外部实例化
   QueueSingleton() {} 

public:
   // 全局访问点，返回单例实例
   static QueueSingleton* getInstance() {
      static QueueSingleton* instance = new QueueSingleton;
      return instance;
   }

   // 添加元素到队列
   void enqueue(int value) {
      std::lock_guard<std::mutex> lock(mtx);
      queue.push(value);
   }

   // 从队列中弹出元素
   int dequeue() {
      std::lock_guard<std::mutex> lock(mtx);
      int frontElement = queue.front();
      queue.pop();
      return frontElement;
   }

   // 获取队列大小
   int size() {
      std::lock_guard<std::mutex> lock(mtx);
      return queue.size();
   }
};

std::mutex QueueSingleton::mtx;

int main() {
   QueueSingleton* queueInstance = QueueSingleton::getInstance();

   queueInstance->enqueue(1);
   queueInstance->enqueue(2);
   queueInstance->enqueue(3);

   std::cout << "Queue size: " << queueInstance->size() << std::endl;

   int dequeuedValue = queueInstance->dequeue();
   std::cout << "Dequeued value: " << dequeuedValue << std::endl;

   std::cout << "Queue size: " << queueInstance->size() << std::endl;

   return 0;
}