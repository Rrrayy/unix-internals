#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>

int main(){
      //  1：进程启动时默认的 fd 0/1/2 
      std::cout << "fd 0 1 2→ stdin（标准输入）stdout（标准输出) stderr（标准错误）" << std::endl;

      //  2：第一次 open，观察 fd 编号
      std::cout << " 第一次 open " << std::endl;
      int fd1 = open("test.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
      std::cout << "open 返回 fd = " << fd1 << std::endl;

      //  3：第二次 open 同一文件 
      std::cout << "第二次 open " << std::endl;
      int fd2 = open("test.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
      std::cout << "第二次 open 返回 fd = " << fd2 << std::endl;

      //  4：验证文件偏移独立 
      std::cout << "fd1 先写 AAAA（偏移从 0到4）" << std::endl;
      write(fd1, "AAAA", 4);
      std::cout << "fd2 再写 BBBB（偏移也从 0 开始）" << std::endl;
      write(fd2, "BBBB", 4);
      // fd1 和 fd2 指向不同文件表项  各管各的偏移 所以 fd2 从 0 覆盖了前 4 字节  文件内容是 BBBB

      close(fd1);
      close(fd2);

      //  5：dup 共享文件偏移 
      int fd3 = open("test2.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
      int fd4 = dup(fd3);  // dup 只复制 fd 编号，指向同一文件表项
      std::cout << "fd3 = " << fd3 << ", fd4 = dup(fd3) = " << fd4 << std::endl;

      write(fd3, "XXXX", 4);  // fd3 写 XXXX，偏移到 4
      write(fd4, "YYYY", 4);  // fd4 共享同一偏移！从 4 开始写
      //fd3 和 fd4 共享文件表项 → 偏移是同一个  所以 fd4 从偏移 4 接着写  文件内容是 XXXXYYYY（不覆盖）

      close(fd3);
      close(fd4);

      // 6：用 stat 查看 inode 信息 
      struct stat sb;
      stat("test.txt", &sb);
      std::cout << "test.txt inode=" << sb.st_ino<< "  size=" << sb.st_size<< "  硬链接数=" << sb.st_nlink << std::endl;

      return 0;
  }
