/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 20:09:12 by zedurak           #+#    #+#             */
/*   Updated: 2026/05/16 13:12:04 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cd_special_control(t_shell *shell)
{
	char *new_path;

	new_path = NULL;
	if (!shell->cmds->argv[1] || (ft_strcmp(shell->cmds->argv[1], "~") == 0))
	{
		new_path = my_little_getenv(shell->env_list, "HOME");
		if (!new_path)
        {
            write(2, "cd: HOME not set\n", 17);
            return (NULL);
        }
		return (new_path);
	}
	else if (ft_strcmp(shell->cmds->argv[1], "-") == 0)
	{
		new_path = my_little_getenv(shell->env_list, "OLDPWD");
		if (!new_path)
        {
            write(2, "cd: OLDPWD not set\n", 20);
            return (NULL);
        }
		return (new_path);
	}
	return (shell->cmds->argv[1]);
}

static void	update_cd_envs(t_shell *shell, char *old_pwd)
{
	char	buf[MAX_PATH];

	update_env_node("OLDPWD", old_pwd, shell);
	if (getcwd(buf, sizeof(buf)))
		update_env_node("PWD", buf, shell);
}

int	builtin_cd(t_shell *shell, int in_pipe)
{
	char	*new_path;
	char	old_pwd[MAX_PATH];
	(void)in_pipe;

	if (shell->cmds->argv[1] && shell->cmds->argv[2])
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (perror("minishell: getcwd"), 1);
	new_path = cd_special_control(shell);
	if (!new_path)
		return (1);
	if (chdir(new_path) == -1)
		return (perror("cd:"), 1);
	if (shell->cmds->argv[1] && (ft_strcmp(shell->cmds->argv[1], "-") == 0))
		printf("%s\n", new_path);
	update_cd_envs(shell, old_pwd);
	return (0);
}
