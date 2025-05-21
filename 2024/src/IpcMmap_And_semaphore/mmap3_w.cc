#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

struct People {
  char name[4];
  int age;
};

const int file_struct_cnt = 5;
const int mem_struct_cnt = 10;

int main(int argc, char* argv[]) {
  int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 00777);
  lseek(fd, sizeof(People) * file_struct_cnt - 1, SEEK_SET);  //文件大小为8*5
  write(fd, "", 1);

  //内存大小为8*10
  People* pmap = (People*)mmap(NULL, sizeof(People) * mem_struct_cnt, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);

  //内存赋值
  for (int i = 0; i < 10; ++i) {
    char c = 'a' + i;
    memcpy((pmap + i)->name, &c, 1);
    (pmap + i)->age = 20 + i;
  }

  printf("initialize over.\n");
  sleep(10);  //等待map_normalfile2读取argv[1]
  if (munmap(pmap, sizeof(People) * 10) != 0) {
    printf("munmap error[%s]\n", strerror(errno));
    return -1;
  }

  return 0;
}