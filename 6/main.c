#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

void get_name(const char *path, const char *name)
{
    // 获取路径文件名或文件夹名
    char *s = strrchr(path, '/');
    sscanf(s, "/%s", name);
}

void copy_file(const char *source, const char *target)
{
    int fd[2];
    fd[0] = open(source, O_RDONLY);
    if (fd[0] == -1)
    {
        return;
    }

    struct stat msg;
    if (stat(target, &msg) == -1)
    {
        // 目标路径不存在，默认该路径为文件
        fd[1] = open(target, O_CREAT|O_RDWR, 0666);
    }else
    {
        if (S_ISDIR(msg.st_mode))
        {
            // 目标路径是文件夹
            char file_name[32];
            get_name(source, file_name);
            char s[100];
            sprintf(s, "%s/%s", target, file_name);
            // printf(s);
            // printf("\n");
            fd[1] = open(s, O_CREAT|O_RDWR|O_TRUNC, 0666);
        }else
        {
            // 目标路径是文件
            fd[1] = open(target, O_RDWR, 0666);
        }
    }

    if (fd[1] == -1)
    {
        return;
    }
    char buf[1024];
    int read_len;
    while ((read_len = read(fd[0], buf, 1024)) > 0)
    {
        if (write(fd[1], buf, read_len) == -1)
        {
            return;
        }
    }
    close(fd[0]);
    close(fd[1]);
}

void copy_dir(const char *source, const char *target)
{
    // 复制源路径指向的整个文件夹，而不是复制该文件夹中的所有内容
    DIR *source_dir = opendir(source);
    if (source_dir == NULL)
    {
        return;
    }

    char s[128], t[128], dir_name[32];
    get_name(source, dir_name);
    sprintf(t, "%s/%s", target, dir_name);
    if (access(t, F_OK) == -1)
    {
        // 目标路径对应的文件夹不存在
        mkdir(t, 0777);
    }

    struct dirent *dirp;
    while (1)
    {
        dirp = readdir(source_dir);
        if (dirp == NULL)
        {
            break;
        }

        if(dirp->d_type == DT_DIR)
        {
            // 子目录是文件夹
            // printf("%s是一个目录\n",dirp->d_name);
            if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
            {
                continue;
            }
            sprintf(s, "%s/%s", source, dirp->d_name);
            sprintf(t, "%s/%s", target, dir_name);
            copy_dir(s, t);
        }
        else if(dirp->d_type == DT_REG)
        {
            // 子目录是文件
            // printf("%s 是一个普通文件\n",dirp->d_name);
            sprintf(s, "%s/%s", source, dirp->d_name);
            sprintf(t, "%s/%s/%s", target, dir_name, dirp->d_name);
            copy_file(s, t);
        }else
        {
            break;
        }
    }
}

int main(int argc, char const *argv[])
{
    struct stat msg;
    if (stat(argv[1], &msg) == -1) {
        return 0;
    }

    if (S_ISDIR(msg.st_mode))
    {
        // 源路径是文件夹
        copy_dir(argv[1], argv[2]);
    }else
    {
        // 源路径是文件
        copy_file(argv[1], argv[2]);
    }
    return 0;
}
