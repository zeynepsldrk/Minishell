/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_external.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:48:36 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/06 20:39:09 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*F_OK: Dosya/Yol sistemde mevcut mu? (Sadece varlığını kontrol eder).

X_OK: Çalıştırma (execute) yetkin var mı?*/

void	external_in_pipe(int in_pipe, t_shell *shell, char *path)
{
	if (in_pipe) //buradaki sinyal işini spawn içinde yapıyorum zaten
	{
		if (apply_redir(shell->cmds->redirects))
		{
			free(path);
			if (g_signal == SIGINT)
				exit(130);
			exit(1);
		}
		ft_check_env(shell);
		execve(path, shell->cmds->argv, shell->env);
		perror("execve fail");
		free(path);
		exit(127);
	}
}

void	external_none_pipe_child(char *path, t_shell *shell)
{
	//redirler de hata varsa execve çalışmamalı, o yüzden
	//redirler de hata var mı diye kontrol etmek lazım
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redir(shell->cmds->redirects))
	{
		free(path);
		exit(1);
	}
	ft_check_env(shell);
	if (execve(path, shell->cmds->argv, shell->env) == -1)
	{
		perror("execve fail");
		free(path);
		exit(127);
	}
}

void	external_none_pipe_parent(t_shell *shell)
{
	int		how_died;

	how_died = 0;
	if (wait(&how_died) == -1)
	{
		perror("wait fail");
		exit(1);
	}
	if (g_signal == SIGINT)
	{
		shell->exit_value = 130;
		g_signal = 0;
	}
	signal(SIGINT, works_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(how_died))
		shell->exit_value = WEXITSTATUS(how_died);
	else if (WIFSIGNALED(how_died))
		shell->exit_value = 128 + WTERMSIG(how_died);
}

void	external_none_pipe(t_shell *shell, char *path)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork fail");
		exit(1);
	}
	if (pid == 0)
	{
		external_none_pipe_child(path, shell);
	}
	else
	{
		free(path);
		external_none_pipe_parent(shell);
		 /*wait() kullanmazsak child process zombie process olarak öylece kalır. Arka planda
		boş yere yer kaplayan processler olur. İşletim sisteminin de belirli sayıda child
		process açma hakkı olduğu için ileride sıkıntı oluşturur.*/
	}
}

void	execute_external(t_shell *shell, int in_pipe)
{
	//externallerin çalıştırılması için child process açılmalıdır.
	/*fork() bir pid döner ve bu pid == 0 ise child process
								 pid == -1 ise child process başarısız
								 pid > 0 parent process*/
	/*The child process uses the same 
	pc(program counter), same CPU registers, and same open files which use in the parent process.*/
	//Ayrıca execve nin argümanlarından komutun yolunu da bulan bir fonksiyon yazmak lazım
	char	*path;

	shell->exit_value = 0;
	path = find_command_path(shell);
	if (!path)
	{
		if (shell->exit_value == 0)
			print_path_error(shell, shell->cmds->argv[0], "command not found", 127);
		if (in_pipe)
			exit(shell->exit_value);
		return ;
	}
	external_in_pipe(in_pipe, shell, path);
	external_none_pipe(shell, path);
}
