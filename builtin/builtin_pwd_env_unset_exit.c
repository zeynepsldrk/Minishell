/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd_env_unset_exit.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 12:43:49 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/21 16:10:31 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_shell *shell, int in_pipe)
{
	char	*pwd;

	(void) shell;
	(void) in_pipe;
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

int	builtin_env(t_shell *shell, int in_pipe)
{
	(void)in_pipe;
	if (shell->cmds->argv[1])
	{
		write(2, "minishell: env: too many arguments\n", 36);
		return (127);
	}
	print_env_list(shell->env_list);
	return (0);
}

int	builtin_unset(t_shell *shell, int in_pipe)
{
	int	i;

	(void) in_pipe;
	i = 1;
	if (!shell->cmds->argv[1])
		return (0);
	while (shell->cmds->argv[i])
	{
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

int	builtin_exit(t_shell *shell, int in_pipe)
{
	unsigned char	exit_code;

	if (!in_pipe)
		write(2, "exit\n", 5);
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
	if (!is_valid_exit_arg(shell->cmds->argv[1]))
	{
		write(2, "exit\n", 5);
		write(2, "minishell: exit: numeric argument required\n", 43);
		return (do_exit(in_pipe, 2));
	}
	exit_code = (unsigned char)ft_atol(shell->cmds->argv[1]);
	return (do_exit(in_pipe, exit_code));
}
