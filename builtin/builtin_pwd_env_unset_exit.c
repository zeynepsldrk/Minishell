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

int builtin_pwd(t_shell *shell)
{
	/*Fonksiyona **NULL ve 0** değerlerini vermek, bellek boyutu hesaplama ve ayırma işini
	tamamen C standart kütüphanesine (glibc) bırakmanızı sağlar. Arka planda kütüphane
	gerekli bellek alanını dinamik olarak ayarlarken, işletim sistemi çekirdeği de mevcut
	dizinin inode numarasından başlayıp kök dizine kadar ağaç tırmanışı yaparak tam
	dosya yolunu adım adım tespit eder.*/
	char *pwd;
    (void)shell;

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

int builtin_env(t_shell *shell)
{
	print_env_list(shell->env_list);
	return (0);
}

int builtin_unset(t_shell *shell)
{
	int i;

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

int builtin_exit(t_shell *shell)
{
	if (!shell->cmds->argv[1])
	{
		write(2, "exit\n", 5);
		exit(shell->exit_value);
	}
	if (shell->cmds->argc > 2)
	{
		write(2, "exit\n", 5);
		write(2, "minishell: exit: too many arguments\n", 37);
		shell->exit_value = 1;
		return 1;
	}
	if (!is_valid_exit_arg(shell->cmds->argv[1])) //exit code nin sayısal olması gerekir
	{
		write(2, "exit\n", 5);
		write(2, "minishell: exit: numeric argument required\n", 43);
		exit(255);
	}
	write(2, "exit\n", 5);
	exit(ft_atol(shell->cmds->argv[1]) % 256);
}
