/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 20:09:12 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/25 20:09:41 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cd_special_control(t_shell *shell, char *new_path)
{
	if (!shell->cmds->argv[1] || ft_strcmp(shell->cmds->argv[1], '~'))
	{
		new_path = my_little_getenv(shell->env_list, "HOME");
		if (!new_path)
			write(2, "cd: HOME not set\n", 18), 1;
        return (new_path);
	}
	else if (ft_strcmp(shell->cmds->argv[1], '-') == 0)
	{
		new_path = my_little_getenv(shell->env_list, "OLDPWD");
		if (!new_path)
			write(2, "cd: OLDPWD not set\n", 20), 1;
        return (new_path);
	}
    return (shell->cmds->argv[1]);
}

int	builtin_cd(t_shell *shell)
{
	char	*new_path;
	char	buf[MAX_PATH];

	if (!getcwd(buf, sizeof(buf)))
		return (perror("minishell: getcwd"), 1);
	new_path = cd_special_control(shell, new_path);
	if (chdir(new_path) == -1)
	{
		perror("cd:");
		return (1);
	}
    if (shell->cmds->argv[1] && (shell->cmds->argv[1], '-') == 0)
        printf("%s\n", new_path);
	update_env_node("OLDPWD", buf, shell);
	my_bzero(buf, MAX_PATH);
	if (getcwd(buf, sizeof(buf)))
		update_env_node("PWD", new_path, shell);
	return (0);
}
