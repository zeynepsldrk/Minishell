/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd_env_unset_exit.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 12:43:49 by zedurak           #+#    #+#             */
/*   Updated: 2026/05/16 13:02:44 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_pwd(t_shell *shell, int in_pipe)
{
	/*Fonksiyona **NULL ve 0** değerlerini vermek, bellek boyutu hesaplama ve ayırma işini
	tamamen C standart kütüphanesine (glibc) bırakmanızı sağlar. Arka planda kütüphane
	gerekli bellek alanını dinamik olarak ayarlarken, işletim sistemi çekirdeği de mevcut
	dizinin inode numarasından başlayıp kök dizine kadar ağaç tırmanışı yaparak tam
	dosya yolunu adım adım tespit eder.*/
	char *pwd;
    (void)shell;
    (void)in_pipe;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}

int builtin_env(t_shell *shell, int in_pipe)
{
	(void)in_pipe;
	print_env_list(shell->env_list);
	return (0);
}

int builtin_unset(t_shell *shell, int in_pipe)
{
	int i;
    (void)in_pipe;

	i = 1;
	if (!shell->cmds->argv[1])
		return (0);
	while (shell->cmds->argv[i])
	{ //geçerli mi diye bakmaya gerek yok env_list de o key i bulamıyorsa silmiyor sıfır ile çıkıyor bashde
		delete_env_node(&shell->env_list, shell->cmds->argv[i]);
		i++;
	}
	return (0);
}

static int	do_exit(int in_pipe, int code)
{
	if (!in_pipe)
		exit(code);
	return (code);
}

int builtin_exit(t_shell *shell, int in_pipe)
{
	if (!shell->cmds->argv[1])
	{
		write(2, "exit\n", 5);
		return (do_exit(in_pipe, shell->exit_value));
	}
	if (shell->cmds->argc > 2)
	{
		write(2, "exit\n", 5);
		write(2, "minishell: exit: too many arguments\n", 37);
		shell->exit_value = 1;
		return (1);
	}
	if (!is_valid_exit_arg(shell->cmds->argv[1])) //exit code nin sayısal olması gerekir
	{
		write(2, "exit\n", 5);
		write(2, "minishell: exit: numeric argument required\n", 43); //hata veren agümanı da ekle!!
        return (do_exit(in_pipe, 255)); // pipe'daysa return
	}
	write(2, "exit\n", 5);
    return (do_exit(in_pipe, ft_atol(shell->cmds->argv[1]) % 256)); // pipe'daysa return
}
