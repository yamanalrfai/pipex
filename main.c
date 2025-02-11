/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalrfai <yalrfai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 14:50:23 by yalrfai           #+#    #+#             */
/*   Updated: 2025/02/11 17:41:38 by yalrfai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	do_cmd(int fd, char *cmd, char **envp)
{
	char		**vector;

	vector = ft_split(cmd, ' ');
	if (!vector)
		ft_exit(fd);
	if (*cmd == 0 || *cmd == ' ')
		thing_in_cmd(vector, fd, 1);
	if (cmd[0] == '/')
	{
		if (!access(cmd, X_OK))
			execve(cmd, vector, envp);
		thing_in_cmd(vector, fd, 0);
		return ;
	}
	cmd = get_path(vector[0], envp, 0);
	if (!cmd)
	{
		thing_in_cmd(vector, fd, 0);
		perror("get_path");
		exit(EXIT_FAILURE);
	}
	execve(cmd, vector, envp);
	thing_in_cmd(vector, fd, 0);
}

void	start_cmd1(t_pip *pip, char **argv, char **envp)
{
	int	tmp_fd;

	close(pip->fd[0]);
	tmp_fd = open(argv[1], O_RDONLY);
	if (tmp_fd == -1)
	{
		perror("open");
		close(pip->fd[1]);
		exit(EXIT_FAILURE);
	}
	dup2(tmp_fd, 0);
	dup2(pip->fd[1], 1);
	close(pip->fd[1]);
	do_cmd(tmp_fd, argv[2], envp);
	perror("execve");
	close(tmp_fd);
	exit(EXIT_FAILURE);
}

void	start_cmd2(t_pip *pip, char **argv, char **envp)
{
	int	tmp_fd;

	close(pip->fd[1]);
	tmp_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd == -1)
	{
		perror("open");
		close(pip->fd[0]);
		exit(EXIT_FAILURE);
	}
	dup2(tmp_fd, 1);
	dup2(pip->fd[0], 0);
	close(pip->fd[0]);
	do_cmd(tmp_fd, argv[3], envp);
	close(tmp_fd);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	start_pipex(t_pip *pip, char **argv, char **envp)
{
	pip->pid1 = fork();
	if (pip->pid1 == -1)
	{
		perror("fork");
		close(pip->fd[0]);
		close(pip->fd[1]);
		exit(EXIT_FAILURE);
	}
	if (pip->pid1 == 0)
		start_cmd1(pip, argv, envp);
	else
	{
		pip->pid2 = fork();
		if (pip->pid2 == -1)
		{
			perror("fork");
			close(pip->fd[0]);
			close(pip->fd[1]);
			exit(EXIT_FAILURE);
		}
		if (pip->pid2 == 0)
			start_cmd2(pip, argv, envp);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pip	pip;

	if (argc != 5)
	{
		perror("Usage: ./pipex file1 cmd1 cmd2 file2\n");
		exit(EXIT_FAILURE);
	}
	if (pipe(pip.fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	start_pipex(&pip, argv, envp);
	close(pip.fd[0]);
	close(pip.fd[1]);
	wait(NULL);
	wait(NULL);
	return (1);
}
