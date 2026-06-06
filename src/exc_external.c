/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_external.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:48:36 by zedurak           #+#    #+#             */
/*   Updated: 2026/05/16 19:06:42 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*F_OK: Dosya/Yol sistemde mevcut mu? (Sadece varlığını kontrol eder).

X_OK: Çalıştırma (execute) yetkin var mı?*/

char *ft_join_and_free(char *s1, char *s2)
{
	char *res = ft_strjoin(s1, s2);
	free(s1);
	return (res);
}

void ft_free_split(char **arr)
{
	int i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char *check_the_path(char *path, char *cmd)
{
	char **splited_path;
	char *full_path;
	int  i;

	splited_path = ft_split(path, ':');
	i = 0;
	while (splited_path[i])
	{
		full_path = ft_strjoin(splited_path[i], "/");
		full_path = ft_join_and_free(full_path, cmd);
		if (access(full_path, F_OK) == 0)
		{
			ft_free_split(splited_path);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_split(splited_path);
	return (NULL);
}

void	print_path_error(char *cmd, char *msg, int exit_code)
{
	write(2, "minishell: ", 11);
	if (cmd && *cmd)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": ", 2);
	}
	if (msg)
		write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

int	is_path_okey(char *path)
{
	struct stat st;
	
	if (access(path, F_OK)) //dosya disk de var mı?
	{
		print_path_error(path, "command not found", 127);
		return (1);
	}
	stat(path, &st);
	if (S_ISDIR(st.st_mode)) // Klasör mü, dosya mı?
	{
		print_path_error(path, "is a directory", 126);
		return (1);
	}
	if (access(path, X_OK)) //çalıştırılabilir mi?
	{
		print_path_error(path, "Permission denied", 126);
		return (1);
	}
	return (0);
}

int	is_absolute_path(char *path)
{
	if (!path || !path[0])
		return (1);
	if (path[0] == '/')
		return (0);
	if (path[0] == '.')
	{
		if (path[1] == '/' || (path[1] == '.' && path[2] == '/'))
			return (0);
	}
	return (1);
}

char	*find_command_path(t_shell *shell)
{
	char	*path;

	if (is_absolute_path(shell->cmds->argv[0]) == 0)
	{
		if (is_path_okey(shell->cmds->argv[0]) == 0)
			return (ft_strdup(shell->cmds->argv[0]));
		return (NULL);
	}
	path = my_little_getenv(shell->env_list, "PATH");
	if (!path)
		return (NULL);
	path = check_the_path(path, shell->cmds->argv[0]);
	if (path && (is_path_okey(path) == 0))
		return (path);
	free(path);
	return (NULL);
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
	int		how_died;
	char	*path;
	pid_t	pid;

	how_died = 0;
	path = find_command_path(shell);
	if (!path)
    {
        shell->exit_value = 127;
        if (in_pipe)
            exit(127);
        return ;
    }
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
	pid = fork();
	if (pid < 0)
	{
		perror("fork fail");
		exit(1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		//redirler de hata varsa execve çalışmamalı, o yüzden redirler de hata var mı diye kontrol etmek lazım
		if (apply_redir(shell->cmds->redirects))
		{
			free(path);
			exit(1);
		}
		ft_check_env(shell);
		if (execve(path, shell->cmds->argv, shell->env))
		{
			perror("execve fail");
			exit(1);
		}
	}
	else
	{
		free(path);
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
		 /*wait() kullanmazsak child process zombie process olarak öylece kalır. Arka planda
		boş yere yer kaplayan processler olur. İşletim sisteminin de belirli sayıda child
		process açma hakkı olduğu için ileride sıkıntı oluşturur.*/
	}
}
