#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
// #include <sys/types.h>

int main(int argc, char const *argv[])
{
    int fd[2];
    int rt = pipe(fd);
    pid_t pid;

    if (rt == -1)
    {
        return 0;
    }

    pid = fork();
    if (pid < 0)
    {
        return 0;
    }
    else if (pid == 0)
    {
        // 子进程1
        char s[] = "Child process 1 is sending a message!\n";
        close(fd[0]);
        write(fd[1], s, 38);// 此处填38，是为了不写入msg最后的'\0'
        exit(0);
    }

    waitpid(pid, NULL, 0);
    pid = fork();
    if (pid < 0) {
        return 0;
    }
    else if (pid == 0)
    {
        // 子进程2
        char s[] = "Child process 2 is sending a message!\n";
        close(fd[0]);
        write(fd[1], s, 39);
        exit(0);
    }

    waitpid(pid, NULL, 0);
    close(fd[1]);
    char s[100];
    read(fd[0], s, 100);
    printf(s);

    return 0;
}
