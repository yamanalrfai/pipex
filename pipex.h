#ifndef PIPEX_H
# define PIPEX_H
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft/libft.h"

typedef struct s_pip
{
    int fd[2];
    pid_t pid1;
    pid_t pid2;
} t_pip;

void	ft_exit(int fd);
void	words_free(char **str);
#endif