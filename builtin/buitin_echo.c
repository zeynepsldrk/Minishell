/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitin_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 20:04:36 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/06 18:39:51 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*echo komutu için -n, -e ve -E flaglerinin kombinasyonlarında davranışlarını belirledim. */
int	info_flags(char *arg)
{
	int	i;

	i = 1;
	if (!arg || arg[0] != '-' || arg[1] == '\0')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(t_shell *shell, int in_pipe)
{
	int	i;
	int	n_flag;

    (void)in_pipe;
	i = 1;
	n_flag = 0;
	while (shell->cmds->argv[i] && info_flags(shell->cmds->argv[i]))
	{
		n_flag = 1;
		i++;
	}
	while (shell->cmds->argv[i])
	{
		printf("%s", shell->cmds->argv[i]);
		if (shell->cmds->argv[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (0);
}
