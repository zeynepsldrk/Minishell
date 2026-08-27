/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_working_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 14:50:44 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/21 18:04:22 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_shell *shell)
{
	if (!shell->cmds->argv || !shell->cmds->argv[0])
	{
		exit(0);
	}
	if (is_builtin(shell->cmds->argv[0], shell))
	{
		execute_builtin(shell->cmds->argv[0], shell, 0, 1);
		exit(shell->exit_value);
	}
	else
		execute_external(shell, 1);
}

void	execute_child_logic(t_shell *shell, t_cmd *cmd, int i)
{
	shell->cmds = cmd;
	connect_child_fds(i, shell->pipes.command_count, shell->pipes.fd);
	if (apply_redir(shell->cmds->redirects))
	{
		if (g_signal == SIGINT)
			exit(130);
		exit(1);
	}
	execute_command(shell);
	exit(127);
}

void	spawn_commands(t_shell *shell, pid_t *pid, int i)
{
	t_cmd	*cmd;

	cmd = shell->cmds;
	while (shell->pipes.command_count > i)
	{
		pid[i] = fork();
		if (pid[i] == -1)
		{
			perror("fork");
			ft_free_pipes(shell->pipes.fd, shell->pipes.pipe_count);
			while (--i >= 0)
				waitpid(pid[i], NULL, 0);
			free(pid);
			exit(1);
		}
		if (pid[i] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			execute_child_logic(shell, cmd, i);
		}
		cmd = cmd->next;
		i++;
	}
}
