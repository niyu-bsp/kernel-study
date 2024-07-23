#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#define DEV_NAME    ("/dev/test1")
char str[100] = {"hello world"};
char buf[100] = { 0 };


int main(int argc, char **argv)
{
    int fd = 0, ret = 0;
    fd = open(DEV_NAME, O_RDWR);
    if(fd == -1)
    {
        printf("open %s failed\n", DEV_NAME);
        return -1;
    }
    while(1)
    {
        sleep(1);
    }
#if 1
    ret = write(fd, str, sizeof(str));
    if( ret != 0 )
    {
        printf("write failed %d\n", ret);
        return -1;
    }
    ret = read(fd, buf, sizeof(buf));
    if( ret != 0)
    {
        printf("read failed %d\n", ret);
        return -1;
    }
    printf("readback:%s\n", buf);
#else
    struct data_t{
        char name[10];
        int age;
    }data =
    {
        .name = "niyu",
        .age = 20,
    };
   
    #define WR_DATA _IOW('a', 1, struct data_t)
    #define RD_DATA _IOR('a', 1, struct data_t)
    struct data_t result = { 0 };
    ret = ioctl(fd, WR_DATA, &data);
    ret = ioctl(fd, RD_DATA, &result);
    printf("name:%s, age %d\n", result.name, result.age);

#endif
}
