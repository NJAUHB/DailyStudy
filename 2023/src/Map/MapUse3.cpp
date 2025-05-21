// C++ program for illustration
// of map::find() function
#include <map>
#include <iostream>
using namespace std;

int main() {

  // initialize container
  multimap<int, int> mp;

  // insert elements in random order
  mp.insert({2, 30});
  mp.insert({1, 40});
  mp.insert({3, 20});
  mp.insert({4, 50});

  cout << "The elements from position 3 in map are : \n";
  cout << "KEY\tELEMENT\n";

  // find() function finds the position at which 3 is
  for (auto itr = mp.find(3); itr != mp.end(); itr++)
    std::cout << itr->first << '\t' << itr->second << '\n' << std::endl;

  std::map<int, int>::iterator it = mp.find(1);
  if (it != mp.end()) {
    std::cout << it->second << std::endl;  // 输出40
  }
  it = mp.find(5);
  if (it == mp.end()) {
    std::cout << "not found" << std::endl;  // 输出not found
  }

  return 0;
}