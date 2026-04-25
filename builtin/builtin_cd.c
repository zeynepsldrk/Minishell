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

void	update_path(char *key, char *value, t_shell *shell)
{
	int			i;//shell içinde ki env_list in içerisinde gezineceğim ve key olarak verilen değeri gödüğümüzde onu value ile değiştireceğiz.
	t_env_node	*temp;

	i = 0;
	temp = shell->env_list;
	while (temp->next)
	{
		if (str_cmp(temp->key, key) == 0)
		{
			if (temp->value)
				free(temp->value);
			temp->value = str_dup(value);
		}
		temp = temp->next;
	}
}

void	cd_special_control(t_shell *shell, char *new_path)
{
	if (!shell->cmds->argv[1] || str_cmp(shell->cmds->argv[1], '~'))
	{
		new_path = my_little_getenv(shell->env_list, "HOME");
		if (!new_path)
			return (write(2, "cd: HOME not set\n", 18), 1);
	}
	else if (str_cmp(shell->cmds->argv[1], '-') == 0)
	{
		new_path = my_little_getenv(shell->env_list, "OLDPWD");
		if (!new_path)
			return (write(2, "cd: OLDPWD not set\n", 20), 1);
		printf("%s\n", new_path);
	}
	else
		new_path = shell->cmds->argv[1];
}

int	builtin_cd(t_shell *shell)
{
	char	*new_path;
	char	buf[MAX_PATH];

	if (!getcwd(buf, sizeof(buf)))
		return (perror("minishell: getcwd"), 1);
	cd_special_control(shell, new_path);
	if (chdir(new_path) == -1)
	{
		perror("cd:");
		return (1);
	}
	update_path("OLDPWD", buf, shell);
	my_bzero(buf, MAX_PATH);
	if (getcwd(buf, sizeof(buf)))
		update_path("PWD", new_path, shell);
	return (0);
}
