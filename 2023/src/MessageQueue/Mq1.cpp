#include <iostream>
#include <cstring>  // strlen
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <thread>

using namespace std;

class MyQueue {
 private:
  MyQueue() { cout << "构造函数" << endl; };
  ~MyQueue();
  mqd_t my_queue_id;  //创建消息队列描述符
  const char* mq_name;

 public:
  static MyQueue* GetMyqueu();
  mqd_t CreateMQ(const char* name);
  bool SendMsg(const char* msg_ptr);
  bool GetMsg(char* msg_ptr, size_t msg_len);
};

MyQueue::~MyQueue() {
  std::cout << "删除并关闭队列" << std::endl;
  mq_unlink(mq_name);
  mq_close(my_queue_id);
}

MyQueue* MyQueue::GetMyqueu() {
  static MyQueue mq;
  return &mq;
}

inline mqd_t MyQueue::CreateMQ(const char* name) {
  mq_name = name;
  my_queue_id = mq_open(name, O_RDWR | O_CREAT | O_EXCL, 0666, NULL);
  if (my_queue_id < 0) {
    if (errno == EEXIST) {
      cout << "!!!errno == EEXIS!!!" << strerror(errno) << endl;
      mq_unlink(name);
      my_queue_id = mq_open(name, O_RDWR | O_CREAT, 0666, NULL);
    } else {
      cout << "open message queue error..." << strerror(errno) << endl;
      return -1;
    }
  }
  return my_queue_id;
}

inline bool MyQueue::SendMsg(const char* msg_ptr) {
  if (mq_send(my_queue_id, msg_ptr, strlen(msg_ptr), 0) == 0) {
    return true;
  } else {
    return false;
  }
}

inline bool MyQueue::GetMsg(char* msg_ptr, size_t msg_len) {
  if (mq_receive(my_queue_id, msg_ptr, msg_len, NULL) < 0) {
    return false;
  } else {
    std::cout << "GetMsg---" << msg_ptr << std::endl;
    return true;
  }
}

int main() {
  char QName[] = "/q_hello"; /* linux必须是/filename这种格式，不能出现二级目录 */
  MyQueue* mq = MyQueue::GetMyqueu();
  int qid = mq->CreateMQ(QName);
  if (qid < 0) {
    std::cout << "创建队列失败" << std::endl;
    return 0;
  } else {
    std::cout << "创建队列成功" << std::endl;
  }
  char msg[] = "Hello Hi Thanks";
  if (fork() == 0) {  //子进程
    mq_attr mqAttr;
    mq_getattr(qid, &mqAttr);
    char* buf = new char[mqAttr.mq_msgsize];  //获得消息队列的大小
    for (int i = 1; i <= 5; ++i) {
      if (mq->GetMsg(buf, mqAttr.mq_msgsize)) {
        std::cout << "GetMsg Successful" << std::endl;
        continue;
      } else {
        std::cout << "GetMsg Failed" << std::endl;
        continue;
      }
    }
    delete[] buf;  // 使用完之后释放buf
    _exit(
        0); /*如果使用exit(0)；子进程的所有静态、全局对象都会被析构掉一次，这就导致了析构函数被调用了两次。这是非常不合理的！
                  *可以通过修改子进程终止的方式来实现，替换exit(0)，使用_exit(0)或者_Exit(0)，这两个方法会立即结束进程，而不清理局部静态对象。   
                  */
  }
  for (int i = 1; i <= 5; ++i) {
    if (mq->SendMsg(msg)) {
      cout << "send msg ->" << msg << "<- successful " << i << "time" << endl;
    } else {
      cout << "send failed " << i << "time" << endl;
    }
    sleep(1);
  }
}