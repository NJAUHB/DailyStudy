#include <iostream>
#include <unordered_map>
#include <list>
#include <memory>
#include <string>
#include <cstring>

// 基类定义
class EntityInfo {
 public:
  ~EntityInfo() = default;
  // virtual void display() const = 0;
  uint16_t type;
};

// 派生类定义
class EntityInfoV1_1 : public EntityInfo {
 public:
  int specific_member_v1;
  EntityInfoV1_1(int val) : specific_member_v1(val) {
    char a[1] = {'a'};
    static_cast<void>(memcpy(&topic, a, 127));
  }

  // void display() const override {
  //   std::cout << "EntityInfoV1_1, specific_member_v1: " << specific_member_v1 << std::endl;
  // }

 public:
  std::string name {"tom"};
  char topic[128];
};

class EntityInfoV2_0 : public EntityInfo {
 public:
  std::string specific_member_v2;

  EntityInfoV2_0(const std::string& val) : specific_member_v2(val) {
    char b[1] = {'b'};
    static_cast<void>(memcpy(&topic, b, 127));
  }

  // void display() const override {
  //   std::cout << "EntityInfoV2_0, specific_member_v2: " << specific_member_v2 << std::endl;
  // }

 public:
  std::string name {"bob"};
  uint16_t type = 12U;
  char topic[128];
};
//**********************************
// 测验asan报错：resp_msg->e2e_info.local_info = *(std::static_pointer_cast<EntityInfoV1>(others));
// 将 EntityInfoBase 指针强制转换为 EntityInfoV1 指针，并尝试解引用这个指针时，可能会访问到未分配或未初始化的内存区域。
class EntityInfoBase {
 public:
  ~EntityInfoBase() = default;
  char name[128];
  uint16_t type;
};

class EntityInfoV3_0 : public EntityInfoBase {
  public:
    uint8_t reserved[32];
};
//**********************************
int main() {
  // 类的长度
  std::cout << "size of EntityInfo = " << sizeof(EntityInfo) << std::endl;
  std::cout << "size of EntityInfoV1_1 = " << sizeof(EntityInfoV1_1) << std::endl;
  std::cout << "size of EntityInfoV2_0 = " << sizeof(EntityInfoV2_0) << std::endl;

  std::unordered_map<std::string, std::list<std::shared_ptr<EntityInfo>>> widedomain_list_map;

  // 创建不同类型的 EntityInfo 对象
  std::shared_ptr<EntityInfo> info1 = std::make_shared<EntityInfoV1_1>(42);
  std::shared_ptr<EntityInfo> info2 = std::make_shared<EntityInfoV2_0>("example");
  std::cout << "info2 type = " << info2->type << std::endl;
  // 将这些对象添加到 map 中的同一个键下
  widedomain_list_map["key1"].push_back(info1);
  widedomain_list_map["key1"].push_back(info2);

  // 遍历并显示存储的 EntityInfo 对象
  for (const auto& item : widedomain_list_map["key1"]) {
    // item->display();
    //std::cout << item->name << std::endl;//编译报错：error: ‘using element_type = class EntityInfo’ {aka ‘class EntityInfo’} has no member named ‘name’
    std::cout << item->type << std::endl;  // 输出两次0
    // 访问特定的派生类成员变量
    /*
    item 是一个 std::shared_ptr<EntityInfo> 类型，指向一个 EntityInfo 对象的实例（可能是 EntityInfoV1_1 或 EntityInfoV2_0 的实例）。

    std::dynamic_pointer_cast<EntityInfoV1_1>(item) 尝试将 item 转换为 std::shared_ptr<EntityInfoV1_1> 类型的指针。
    如果 item 实际上指向一个 EntityInfoV1_1 对象，那么转换成功，v1_1_ptr 将指向这个对象；否则，v1_1_ptr 将是空指针。
    如果 v1_1_ptr 非空，意味着 item 成功转换为 EntityInfoV1_1 类型，可以安全地访问 EntityInfoV1_1 特有的成员变量 specific_member_v1 和 name。

    如果 v1_1_ptr 是空指针，意味着 item 不是指向 EntityInfoV1_1 类型的对象。
    
    接着尝试 std::dynamic_pointer_cast<EntityInfoV2_0>(item)，看是否能将 item 转换为 EntityInfoV2_0 类型的指针。
    如果 v2_0_ptr 非空，意味着 item 成功转换为 EntityInfoV2_0 类型，可以安全地访问 EntityInfoV2_0 特有的成员变量 specific_member_v2、type 和 name。
    
    能否改成static_pointer_cast?
    static_pointer_cast 不会返回空指针，所以实际的代码不能简单替换！
    若改成static_pointer_cast，下面的else if分支就不会走到
    */
    if (auto v1_1_ptr = std::dynamic_pointer_cast<EntityInfoV1_1>(item)) {
      std::cout << "Accessed specific_member_v1: " << v1_1_ptr->specific_member_v1 << std::endl;
      std::cout << v1_1_ptr->name << std::endl;
    } else if (auto v2_0_ptr = std::dynamic_pointer_cast<EntityInfoV2_0>(item)) {
      std::cout << "Accessed specific_member_v2: " << v2_0_ptr->specific_member_v2 << std::endl;
      std::cout << v2_0_ptr->type << std::endl;
      std::cout << v2_0_ptr->name << std::endl;
    }
  }

  //**********************************
  // 测验asan报错
  std::shared_ptr<EntityInfoV3_0> info3 = std::make_shared<EntityInfoV3_0>();
  info3->reserved;
  info3->name;
  info3->type;
  std::shared_ptr<EntityInfoBase> info3_ = std::make_shared<EntityInfoBase>(*info3);
  info3_->name;
  info3_->type;
  std::shared_ptr<EntityInfoBase> iNfO3_ = std::make_shared<EntityInfoV3_0>();
  // iNfO3_->reserved;
  // info3_->reserved;
  std::shared_ptr<EntityInfoV3_0> INFO3 = std::static_pointer_cast<EntityInfoV3_0>(info3_);//结论：正常编译不会存在报错，asan会报错
  // INFO3->reserved;
  INFO3->name;
  INFO3->type;

  //**********************************
  return 0;
}
