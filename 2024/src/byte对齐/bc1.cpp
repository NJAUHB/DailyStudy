
#include <iostream>
#include <type_traits>
#include <cstddef>

typedef enum ReliabilityQosPolicyKind : unsigned char {
  BEST_EFFORT_RELIABILITY_QOS = 0x01,
  RELIABLE_RELIABILITY_QOS = 0x02
} ReliabilityQosPolicyKind;
enum QueueWakeUp { PTHREAD_CONDITION_WAKE_UP, DTS_WAKE_UP };
enum DiscoveryType : int8_t { STATIC_DISCOVERY, DYNAMIC_DISCOVERY };
enum EntityType : int8_t { ENTITY_WRITER, ENTITY_READER, ENTITY_TYPE_UNKNOW };
enum EntityClassType : uint8_t { EntityType1 };

struct EntityId_s {
  uint8_t value[4];
};
enum EntityStatus : int8_t { ENTITY_ONLINE, ENTITY_OFFLINE, ENTITY_CRASH, ENTITY_MATCH };

struct GuidPrefix_s {
  uint8_t value[12];
  static bool is_fix;
};

struct GUID_s {
  GuidPrefix_s guidPrefix;
  EntityId_s entityId;
};
typedef struct entity_info_v1 {
  // GUID_s guid; // 16
  // char entity_version[64]; // 1 * 64
  // char topic_name[128]; // 1 * 128
  // char data_type[128]; // 1 * 128
  // char recv_queue[256]; // 1 * 256
  // uint64_t timestamp; // 8
  // QueueWakeUp queue_wake_up; // 4
  // uint32_t queue_msg_num; // 4
  // uint32_t pool_id; // 4
  // ReliabilityQosPolicyKind kind; // 1
  // DiscoveryType discovery_type; // 1
  // int8_t status; // 1
  // int8_t entity_type; // 1
  GUID_s guid;         // 16
  uint16_t domain_id;  // 2
  char entity_version[64];
  char topic_name[128];
  char data_type[128];
  char recv_queue[256];
  ReliabilityQosPolicyKind kind;  // 1
  DiscoveryType discovery_type;   // 1
  uint32_t pool_id;               // 4
  uint64_t timestamp;             // 8
  QueueWakeUp queue_wake_up;      // 4
  uint32_t queue_msg_num;         // 4
  EntityStatus status;            // 1
  uint8_t reserved[31];

  bool operator==(const GUID_s guid) { return true; }
} EntityInfoV1;

typedef struct discovery_header {
  uint16_t proto_version;  // 2
  uint8_t payload_type;    // 1
  uint32_t payload_len;    // 4
  uint32_t seq_num;        // 4
  uint32_t reserved[4];    // 16
} DiscoveryHeader;

class base_e {
 public:
  base_e() = default;
  ~base_e() = default;
  GUID_s guid;
  EntityClassType entity_class_type;
  EntityStatus status;
  ReliabilityQosPolicyKind kind;
  DiscoveryType discovery_type;
  char build_version[64];
  char topic_name[128];
  char data_type[128];
  char recv_queue[256];
  uint32_t pool_id;
  QueueWakeUp queue_wake_up;
  uint64_t timestamp;
  uint32_t queue_msg_num;
};

class e : public base_e {
 public:
  uint8_t reserved[32];
};

int main() {
  std::cout << "base 长度" << sizeof(base_e) << std::endl;
  std::cout << "子类 长度" << sizeof(e) << std::endl;
  EntityInfoV1 ev1;
  std::cout << "sizeof (DiscoveryHeader) = " << sizeof(DiscoveryHeader) << std::endl;
  std::cout << "Alignment of DiscoveryHeader: " << alignof(DiscoveryHeader) << std::endl;
  std::cout << sizeof(GUID_s) << std::endl;
  std::cout << "sizeof (EntityInfoV1) = " << sizeof(EntityInfoV1) << std::endl;
  std::cout << "Alignment of EntityInfoV1: " << alignof(EntityInfoV1) << std::endl;
  /*
    /* 注意：当EntityInfoV1是一个class且继承自一个虚基类时，会出现 offsetof(EntityInfoV1, guid) 的结果从0变成了8。
    /* 
    /* why: 因为当 EntityInfoV1 作为一个类从 EntityInfoBase 继承的时候，编译器为了实现虚函数（也就是支持多态），
    /* 需要在类的内存布局中添加一个指向虚函数表（vtable）的指针。对于大多数的编译器和平台，这个指针的大小是8字节。
    /* 
    /* 虚函数表是一个由函数指针组成的数组，编译器在编译时创建。
    /* 当一个类被声明为包含或者继承了虚函数时，编译器会为这个类创建一个虚函数表，并在每个这个类的对象中添加一个指向这个表的指针。
    /* 当我们通过基类的指针或引用调用虚函数时，编译器会使用这个指针找到正确版本的函数来调用，从而实现多态
    */
  std::cout << " Offset of ' guid': " << offsetof(EntityInfoV1, guid)
            << " Offset of ' domain_id': " << offsetof(EntityInfoV1, domain_id)
            << " Offset of ' entity_version': " << offsetof(EntityInfoV1, entity_version)
            << " Offset of ' topic_name': " << offsetof(EntityInfoV1, topic_name)
            << " Offset of ' data_type': " << offsetof(EntityInfoV1, data_type)
            << " Offset of ' recv_queue': " << offsetof(EntityInfoV1, recv_queue)
            << " Offset of ' kind': " << offsetof(EntityInfoV1, kind)
            << " Offset of ' discovery_type': " << offsetof(EntityInfoV1, discovery_type)
            << " Offset of ' pool_id': " << offsetof(EntityInfoV1, pool_id)
            << " Offset of ' timestamp': " << offsetof(EntityInfoV1, timestamp)
            << " Offset of ' queue_wake_up': " << offsetof(EntityInfoV1, queue_wake_up)
            << " Offset of ' queue_msg_num': " << offsetof(EntityInfoV1, queue_msg_num)
            << " Offset of ' status': " << offsetof(EntityInfoV1, status)
            << " Offset of ' reserved': " << offsetof(EntityInfoV1, reserved) << std::endl;
  return 0;
}
