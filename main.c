/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalrfai <yalrfai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 14:50:23 by yalrfai           #+#    #+#             */
/*   Updated: 2025/02/09 15:20:59 by yalrfai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	get_env(char **cmd, char ***paths, char **envp)
{
	int	i;

	*cmd = ft_strjoin("/", *cmd);
	if (!*cmd)
		return (0);
	i = 0;
	*paths = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			*paths = ft_split(envp[i] + 5, ':');
			if (!*paths)
				return (0);
			return (1);
		}
		i++;
	}
	return (0);
}

char	*get_path(char *cmd, char **envp, int i)
{
	char	**paths;
	char	*joined_cmd;
	char	*tmp;

	paths = NULL;
	joined_cmd = NULL;
	if (!get_env(&cmd, &paths, envp))
		return (NULL);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], cmd);
		if (!tmp)
			return (NULL);
		if (!access(tmp, X_OK))
		{
			joined_cmd = tmp;
			break ;
		}
		free(tmp);
		i++;
	}
	free(cmd);
	words_free(paths);
	return (joined_cmd);
}

void	do_cmd(int fd, char *cmd, char **envp)
{
	char		**vector;

	vector = ft_split(cmd, ' ');
	if (!vector)
		ft_exit(fd);
	if (*cmd == 0 || *cmd == ' ')
	{
		words_free(vector);
		errno = EINVAL;
		ft_exit(fd);
	}
	if (cmd[0] == '/')
	{
		if (!access(cmd, X_OK))
			execve(cmd, vector, envp);
		words_free(vector);
		return ;
	}
	cmd = get_path (vector[0], envp, 0);
	execve(cmd, vector, envp);
	words_free(vector);
}

void    start_cmd1(t_pip *pip, char **argv, char **envp)
{
    int tmp_fd;

    close(pip->fd[0]);
    tmp_fd = open(argv[1], O_RDONLY);
    dup2(tmp_fd, 0);
    dup2(pip->fd[1], 1);
    close(pip->fd[1]);
    do_cmd(tmp_fd, argv[2], envp);
    perror("execve");
    exit(1);
}
void   start_cmd2(t_pip *pip, char **argv, char **envp)
{
    int tmp_fd;

    close(pip->fd[1]);
    tmp_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(tmp_fd, 1);
    dup2(pip->fd[0], 0);
    close(pip->fd[0]);
    do_cmd(tmp_fd, argv[3], envp);
    perror("execve");
    exit(1);
}

void    start_pipex(t_pip *pip, char **argv, char **envp)
{
    pip->pid1 = fork();
    if (pip->pid1 == 0)
        start_cmd1(pip, argv, envp);
    pip->pid2 = fork();
    if (pip->pid2 != 0)
    {
        pip->pid2 = fork();
        if (pip->pid2 == 0)
            start_cmd2(pip, argv, envp);
    }
    else
        perror("fork");
}

int main(int argc, char **argv, char **envp)
{
    t_pip pip;

    if (argc != 5)
        perror("Usage: ./pipex file1 cmd1 cmd2 file2\n");
    if (pipe(pip.fd) == -1)
        perror("pipe");
    start_pipex(&pip, argv, envp);
    close(pip.fd[0]);
    close(pip.fd[1]);
    wait(NULL);
    wait(NULL);
    return (1);
}
