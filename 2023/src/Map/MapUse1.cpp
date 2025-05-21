#include <iostream>
#include <map>

int main() {
  std::map<std::string, uint32_t> name_to_age {{"xiaoming", 0}, {"xiaohong", 0}};
  // []
  name_to_age["xiaoming"] = 12;
  name_to_age["xiaohong"] = 11;
  name_to_age["xiaohang"] = 13;
  std::cout << "xiaoming's age is " << name_to_age["xiaoming"] << std::endl;
  std::cout << "xiaoniu's age is " << name_to_age["xiaoniu"] << std::endl;  // 没有xiaoniu则打印为0或空，
  // at
  std::cout << "xiaoming's age is " << name_to_age.at("xiaoming") << std::endl;
  // name_to_age.at("xiaozhang") = 13; 会发生core
  try {
    std::cout << "xiaoniu's age is " << name_to_age.at("xiaozhang") << std::endl;  // 没有xiaoniu则抛出异常
  } catch (std::exception& e) {
    std::cout << "catch exception: " << e.what() << std::endl;
  }
  return 0;
}