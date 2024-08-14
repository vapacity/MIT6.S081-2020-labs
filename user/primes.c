#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MSGSIZE 36
#define ONE '1'
#define ZERO '0'

/// 递归筛选素数
void prime(int pipe_read) {
    char nums[MSGSIZE];
    int val = -1;

    // 从管道读取数据
    if (read(pipe_read, nums, MSGSIZE) <= 0) {
        close(pipe_read);
        exit(0);
    }

    // 打印读取的字符及其 ASCII 值
    // printf("Read from pipe: ");
    // for (int i = 0; i < MSGSIZE; i++) {
    //     printf("%c(%d) ", nums[i], nums[i]);
    // }
    // printf("\n");

    // 找到第一个素数
    for (int i = 0; i < MSGSIZE; i++) {
        if (nums[i] == ONE) {
            val = i;
            printf("prime %d\n", val);
            nums[i] = ZERO;
            break;
        }
    }

    // 如果没有找到素数，则终止
    if (val == -1) {
        close(pipe_read);
        exit(0);
    }

    // 标记非素数
    for (int i = 0; i < MSGSIZE; i++) {
        if (i % val == 0 && i != val) {
            nums[i] = ZERO;
        }
    }

    // 打印调试信息
    //printf("After marking non-primes with %d: ", val);
    // for (int i = 0; i < MSGSIZE; i++) {
    //     printf("%c", nums[i]);
    // }
    // printf("\n");

    int fd[2];
    pipe(fd);

    // 子进程筛选
    if (fork() == 0) {
        close(fd[1]); // 关闭写端
        prime(fd[0]); // 递归处理新的管道
        exit(0);
    } else {
        close(fd[0]); // 关闭读端
        write(fd[1], nums, MSGSIZE); // 父进程向管道写入数据
        close(fd[1]); // 关闭写端
        wait(0);      // 等待子进程
    }

    close(pipe_read);
    exit(0);
}

int main(int argc, char* argv[]) {
    int fd[2];
    pipe(fd);
    char nums[MSGSIZE];

    // 初始化所有数为素数
    for (int i = 0; i < MSGSIZE; i++) {
        nums[i] = ONE;
    }

    // 父进程先写入数据
    nums[0] = ZERO; // 0 不是素数
    nums[1] = ZERO; // 1 不是素数
    write(fd[1], nums, MSGSIZE);
    close(fd[1]); // 关闭写端

    // 子进程进行素数筛选
    if (fork() == 0) {
        close(fd[1]); // 关闭写端
        prime(fd[0]); // 子进程开始筛选
        exit(0);
    } else {
        close(fd[0]); // 关闭读端
        wait(0);      // 父进程等待子进程完成
    }

    exit(0);
}
