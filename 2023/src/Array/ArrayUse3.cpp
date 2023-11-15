#include <iostream>
int main(){
   char a[3];
   char b[5]={};
   for(auto item:a){
    std::cout << a << std::endl;
   }
   if (a[0] == '\0'){
    std::cout << "0" << std::endl;
   }
   return 0;
}
/*从输出可以看到，输出0为概率性事件
lixiang@PC-S1A14004:~/cpp_ws/DailyStudy/2023/src/Array$ ./a3 
P@�
P@�
P@�
lixiang@PC-S1A14004:~/cpp_ws/DailyStudy/2023/src/Array$ ./a3 
��
�
�
lixiang@PC-S1A14004:~/cpp_ws/DailyStudy/2023/src/Array$ ./a3 
p-
p-
p-
lixiang@PC-S1A14004:~/cpp_ws/DailyStudy/2023/src/Array$ ./a3 
��
��
��
lixiang@PC-S1A14004:~/cpp_ws/DailyStudy/2023/src/Array$ ./a3 



0
*/