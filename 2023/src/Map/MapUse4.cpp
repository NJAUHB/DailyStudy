// map::begin/end
#include <iostream>
#include <map>

int main() {
  std::map<char, int> mymap;

  mymap['b'] = 100;
  mymap['a'] = 200;
  mymap['c'] = 300;

  // begin and end
  for (std::map<char, int>::iterator it = mymap.begin(); it != mymap.end(); ++it) {
    std::cout << (*it).first << " => " << (*it).second << '\n';
    if ((*it).first == 'a') {
      (*it).second = 150;
    }
  }
  /*cbegin and cend: cbegin()和cend()是C++11新增的，它们返回一个const的迭代器，不能用于修改元素
 *const_iterator 对象可以用于const vector 或非 const vector,它自身的值可以改(可以指向其他元素),但不能改写其指向的元素值。
*/
  for (std::map<char, int>::const_iterator it = mymap.cbegin(); it != mymap.cend(); ++it) {
    std::cout << it->first << " => " << it->second << '\n';
    //it->second = 1;//Error
  }
  return 0;
}