#include<iostream>
#include<unistd.h>

int main(){
    std::cout<<"PID = "<<getpid()<<std::endl;
    // 另开终端执行: ps -p 会看到状态是 S+（sleeping，等输入）
    std::cout << "输入..." << std::endl;

    char buf[1024];
    int n = read(STDIN_FILENO, buf, sizeof(buf)-1);
      // 执行，进程挂起, 内核把当前进程从就绪队列移到阻塞队列 状态从 R到 S

    buf[n] = '\0';
    std::cout << "读到 " << n << " 字节: " << buf << std::endl;

    return 0;
}

//PID STAT COMMAND
//  10520 S+   block_read
