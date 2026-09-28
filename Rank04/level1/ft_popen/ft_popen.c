#include <unistd.h>     // pipe, fork, dup2, execvp, close
#include <stdlib.h>     // exit
#include <sys/types.h>  // pid_t

int ft_popen(const char *file, char *const argv[], char type)
{
    if (!file || !argv || (type != 'r' && type != 'w'))
        return(-1);
    int read_end;
    int write_end;
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1)
        return(-1);
    read_end = fd[0];
    write_end = fd[1];
    if (pid == -1)
    {
        close(read_end);
        close(write_end);
        return(-1);
    }
    if (pid == 0)
    {
        if (type == 'r')
        {
            close(read_end);
            if (dup2(write_end, 1) == -1)
                exit(1);
            close(write_end);
        }
        else
        {
            close(write_end);
            if (dup2(read_end, 0) == -1)
                exit(1);
            close(read_end);
        }
        execvp(file, argv);
        exit(1);
    }
    if (type == 'r')
    {
        close(write_end);
        return(read_end);
    }
    close(read_end);
    return(write_end);
}

#pragma region Test code

// Variant from subject without get_next_line and ft_putstr
int main(void)
{
    write(1, "Test ft_popen_short\n", 20);
    int  fd;
    char c;

    fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
    if (fd < 0)
        return (1);
    while (read(fd, &c, 1) > 0)
        write(1, &c, 1);
    close(fd);
        fd = ft_popen("bin/cd", (char *const[]){"cd", "..", NULL}, 'w');
    if (fd < 0)
        return (1);
    while (read(fd, &c, 1) > 0)
        write(1, &c, 1);
    close(fd);
    fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
    if (fd < 0)
        return (1);
    while (read(fd, &c, 1) > 0)
        write(1, &c, 1);
    close(fd);
    return (0);
}
#pragma endregion