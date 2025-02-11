/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:38:51 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/02/11 17:36:19 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_exit(int fd)
{
	if (fd != -1)
		close(fd);
	perror("pipex");
	exit(1);
}

void	words_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

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

void	thing_in_cmd(char **vector, int fd, int exit)
{
	if (vector)
		words_free(vector);
	if (exit)
	{
		errno = EINVAL;
		ft_exit(fd);
	}
	else
	{
		if (fd != -1)
			close(fd);
	}
}
