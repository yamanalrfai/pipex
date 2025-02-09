/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalrfai <yalrfai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 14:48:36 by yalrfai           #+#    #+#             */
/*   Updated: 2025/02/09 14:49:50 by yalrfai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex.h"

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


