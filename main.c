/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 14:50:23 by yalrfai           #+#    #+#             */
/*   Updated: 2025/02/10 11:01:30 by yaman-alrif      ###   ########.fr       */
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
		close(fd);
		ft_exit(fd);
	}
	if (cmd[0] == '/')
	{
		if (!access(cmd, X_OK))
			execve(cmd, vector, envp);
		words_free(vector);
		close(fd);
		return ;
	}
	cmd = get_path(vector[0], envp, 0);
	if (cmd == NULL)
	{
		words_free(vector);
		perror("get_path");
		close(fd);
		exit(EXIT_FAILURE);
	}
	execve(cmd, vector, envp);
	words_free(vector);
	close(fd);
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
