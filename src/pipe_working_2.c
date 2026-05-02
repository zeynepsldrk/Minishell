/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_working_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 14:50:44 by zedurak           #+#    #+#             */
/*   Updated: 2026/05/02 17:31:29 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_command(t_shell *shell)
{
    if (is_builtin(shell->cmds->argv[0], shell->list_builtin))
        execute_builtin(shell->cmds, shell, 0);
    else
        execute_external(shell);
}

void connect_child_fds(int i, int cmd_count, int **fd)
{
    if (i == 0) //ilk komut
        dup2(fd[i][1], STDOUT_FILENO); //stdout'u pipe'ın yazma ucuna yönlendir
    else if (i == cmd_count - 1) //son komut
        dup2(fd[i - 1][0], STDIN_FILENO); //stdin'i önceki pipe'ın okuma ucuna yönlendir
    else //ortadaki komutlar
    {
        dup2(fd[i - 1][0], STDIN_FILENO); //stdin'i önceki pipe'ın okuma ucuna yönlendir
        dup2(fd[i][1], STDOUT_FILENO); //stdout'u pipe'ın yazma ucuna yönlendir
    }
    close_all_pipes(fd, cmd_count - 1, 0); //child process tüm pipe'ları kapatır çünkü artık yönlendirme yapılmıştır
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
            exit(1);
        }
        if (pid[i] == 0) //child process
			execute_child_logic(shell, cmd, i);
        cmd = cmd->next; //her fork da parent bir sonraki node a geçer child o anki cmd ile çalışır
        i++;
    }
}
