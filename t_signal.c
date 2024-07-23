#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#define DEV_NAME    ("/dev/test1")


int done = 0;
void handle_sigint(int sig) {
    if (sig == SIGUSR1)
    {
        printf("APP:get  SIGUSR1 \n");
        done = 1;
    }
}
int main(int argc, char **argv)
{
    int fd = 0, ret = 0;
    struct sigaction sa;

    fd = open(DEV_NAME, O_RDWR);
    if(fd == -1)
    {
        printf("open %s failed\n", DEV_NAME);
        return -1;
    }
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;

    // 使用 sigaction 设置 SIGINT 信号处理程序
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    
    // 进入一个无限循环，等待信号的到来
    printf("App:Waiting for SIGINT \n");
    read(fd, NULL, 0);
    while (done == 0 ) {
       sleep(1);
    }
}
