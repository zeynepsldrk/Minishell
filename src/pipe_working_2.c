/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_working_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 14:50:44 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/06 18:35:50 by zedurak          ###   ########.fr       */
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
		execute_builtin(shell->cmds->argv[0], shell, 0, 1); //pipe içinde çalışacaksa in_pipe 1 olacak
		exit(shell->exit_value);
	}
	else
		execute_external(shell, 1); //child içinde ise 1 olacak
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
	exit(127); //execve başarısızsa buradayız
}

void	spawn_commands(t_shell *shell, pid_t *pid, int i)
{
	t_cmd	*cmd;

	cmd = shell->cmds;
	while (shell->pipes.command_count > i) //komut sayısı - 1 kadar pipe oluştur
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
		if (pid[i] == 0) //child process
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			execute_child_logic(shell, cmd, i);
		}
		cmd = cmd->next; //her fork da parent bir sonraki node a geçer child o anki cmd ile çalışır
		i++;
	}
}
