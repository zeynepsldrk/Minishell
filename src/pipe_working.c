/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_working.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 14:50:44 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/21 20:34:23 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**create_pipes(int pipe_count)
{
	int	**fd;
	int	i;

	fd = malloc(sizeof(int *) * pipe_count);
	if (!fd)
		return (NULL);
	i = 0;
	while (i < pipe_count)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i])
			return (ft_free_pipes(fd, i), NULL);
		if (pipe(fd[i]) == -1)
		{
			free(fd[i]);
			return (ft_free_pipes(fd, i), NULL);
		}
		i++;
	}
	return (fd);
}

void	wait_for_children(t_shell *shell, int *how_died, pid_t *pid)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < shell->pipes.command_count)
	{
		waitpid(pid[i], how_died, 0);
		if (i == shell->pipes.command_count - 1)
			status = *how_died;
		i++;
	}
	if (WIFEXITED(status))
		shell->exit_value = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_value = 128 + WTERMSIG(status);
	free(pid);
}

static void	pipe_working_signal(t_shell *shell, pid_t *pid)
{
	int		how_died;

	how_died = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	wait_for_children(shell, &how_died, pid);
	if (g_signal == SIGINT)
	{
		shell->exit_value = 130;
		g_signal = 0;
	}
	signal(SIGINT, works_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}

void	pipe_working(t_shell *shell)
{
	pid_t	*pid;

	if (shell->pipes.command_count <= 1)
		return ;
	shell->pipes.fd = create_pipes(shell->pipes.pipe_count);
	if (!shell->pipes.fd)
		return ;
	pid = malloc(sizeof(pid_t) * shell->pipes.command_count);
	if (!pid)
	{
		ft_free_pipes(shell->pipes.fd, shell->pipes.pipe_count);
		return ;
	}
	spawn_commands(shell, pid, 0);
	ft_free_pipes(shell->pipes.fd, shell->pipes.pipe_count);
	pipe_working_signal(shell, pid);
}
