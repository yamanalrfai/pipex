/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:25:17 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/02/10 11:30:47 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <sys/wait.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft/libft.h"

typedef struct s_pip
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
}	t_pip;

void	ft_exit(int fd);
void	words_free(char **str);

#endif
