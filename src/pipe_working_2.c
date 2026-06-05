/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_working_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 14:50:44 by zedurak           #+#    #+#             */
/*   Updated: 2026/05/16 13:08:33 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_command(t_shell *shell)
{
    if (!shell->cmds->argv || !shell->cmds->argv[0])
        exit(1);
	if (is_builtin(shell->cmds->argv[0], shell))
    {
		execute_builtin(shell->cmds, shell, 0);
        exit(shell->exit_value);
    }
	else
		execute_external(shell, 1); //child içinde ise 1 olacak
}

static void dup2_with_check(int i, int newfd, int **fd, int pipe_count)
{
    if(ft_safe_dup2(fd[i][1], newfd) == -1)
    {
        ft_free_pipes(fd, pipe_count);
        exit(1);
    }
}

void connect_child_fds(int i, int cmd_count, int **fd)
{
    if ((cmd_count - 1) == 0)
        return;
	if (i == 0) //ilk komut
        dup2_with_check(i, STDOUT_FILENO, fd, cmd_count - 1); //stdout'u pipe'ın yazma ucuna yönlendir
	else if (i == cmd_count - 1) //son komut
        dup2_with_check(i - 1, STDIN_FILENO, fd, cmd_count - 1); //stdin'i önceki pipe'ın okuma ucuna yönlendir
	else //ortadaki komutlar
	{
        dup2_with_check(i - 1, STDIN_FILENO, fd, cmd_count - 1); //stdin'i önceki pipe'ın okuma ucuna yönlendir
		dup2_with_check(i, STDOUT_FILENO, fd, cmd_count - 1); //stdout'u pipe'ın yazma ucuna yönlendir
	}
	ft_free_pipes(fd, cmd_count); //child process tüm pipe'ları kapatır çünkü artık yönlendirme yapılmıştır
}

void execute_child_logic(t_shell *shell, t_cmd *cmd, int i)
{
	shell->cmds = cmd;
	connect_child_fds(i, shell->pipes.command_count, shell->pipes.fd);
	if (apply_redir(shell->cmds->redirects, shell))
		exit(1);
	update_env_nodes(shell);
	execute_command(shell);
	exit(127); //execve başarısızsa buradayız
}

void spawn_commands(t_shell *shell, pid_t *pid, int i)
{
	t_cmd *cmd;

	cmd = shell->cmds;
	while (shell->pipes.command_count > i) //komut sayısı - 1 kadar pipe oluştur
	{
		pid[i] = fork();
		if (pid[i] == -1)
		{
			perror("fork");
            ft_free_pipes(shell->pipes.fd, shell->pipes.pipe_count);
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
