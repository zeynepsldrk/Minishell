/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_working_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 18:37:46 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/21 18:05:40 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	dup2_with_check(int i, int newfd, int **fd, int pipe_count)
{
	int	fd_to_use;

	if (newfd == STDOUT_FILENO)
		fd_to_use = fd[i][1];
	else
		fd_to_use = fd[i][0];
	if (ft_safe_dup2(fd_to_use, newfd) == -1)
	{
		ft_free_pipes(fd, pipe_count);
		exit(1);
	}
}

void	connect_child_fds(int i, int cmd_count, int **fd)
{
	if ((cmd_count - 1) == 0)
		return ;
	if (i == 0)
		dup2_with_check(i, STDOUT_FILENO, fd, cmd_count - 1);
	else if (i == cmd_count - 1)
		dup2_with_check(i - 1, STDIN_FILENO, fd, cmd_count - 1);
	else
	{
		dup2_with_check(i - 1, STDIN_FILENO, fd, cmd_count - 1);
		dup2_with_check(i, STDOUT_FILENO, fd, cmd_count - 1);
	}
	ft_free_pipes(fd, cmd_count - 1);
}
