#include <iostream>
#include <type_traits>
typedef enum ReliabilityQosPolicyKind : unsigned char {
  BEST_EFFORT_RELIABILITY_QOS = 0x01,
  RELIABLE_RELIABILITY_QOS = 0x02
} ReliabilityQosPolicyKind;
enum QueueWakeUp { PTHREAD_CONDITION_WAKE_UP, DTS_WAKE_UP };
enum DiscoveryType : int8_t { STATIC_DISCOVERY, DYNAMIC_DISCOVERY };
typedef struct s1 {
  int64_t A;
  char b[5];
  char B[3];                   // 7
  ReliabilityQosPolicyKind C;  // 1
  QueueWakeUp D;               // 4
  DiscoveryType E;             // 1
} S1;
/*
0       8       15  16     20   21      24
+-------+-------+---+-------+---+-------+
|   A   |   B   | C |   D   | E | pad[3]|
+-------+-------+---+-------+---+-------+
*/
int main() {
  std::cout << "sizeof (S1) = " << sizeof(S1) << std::endl;
  std::cout << "Alignment of S1: " << alignof(S1) << std::endl;
  std::cout << sizeof(std::string) << std::endl;

  return 0;
}