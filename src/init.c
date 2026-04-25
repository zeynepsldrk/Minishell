/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:25:05 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/25 14:17:57 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_builtins(t_builtin *builtins)
{
	builtins[0].name = "cd";
	builtins[0].func = builtin_cd;
	builtins[1].name = "echo";
	builtins[1].func = builtin_echo;
	builtins[2].name = "exit";
	builtins[2].func = builtin_exit;
	builtins[3].name = "pwd";
	builtins[3].func = builtin_pwd;
	builtins[4].name = "export";
	builtins[4].func = builtin_export;
	builtins[5].name = "unset";
	builtins[5].func = builtin_unset;
	builtins[6].name = "env";
	builtins[6].func = builtin_env;
	builtins[7].name = NULL;
	builtins[7].func = NULL;
}

t_shell	*init_shell(t_shell *shell, char **envp)
{
	shell->env_list = NULL;
	shell->env = envp;
	shell->exit_value = 0;
	shell->tokens = NULL;
	shell->cmds = NULL;
	init_builtins(shell->list_builtin); //header dosyasına eklediğim struct ın 
    //içini builtin fonksiyonlarının adları ve çalıştıracağı fonk. atadım. İndex kullanarak fonksiyonları çağırmak istediğimde kolaylık sağlayacak.
	return (shell);
}