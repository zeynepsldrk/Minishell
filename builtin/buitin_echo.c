/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitin_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 20:04:36 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/25 20:08:10 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*echo komutu için -n, -e ve -E flaglerinin kombinasyonlarında davranışlarını belirledim. */
int	print_with_escapes(char *str, int i)
{
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] != '\0')
		{
			if (str[i + 1] == 'n')
				printf("\n");
			else if (str[i + 1] == 't')
				printf("\t");
			else if (str[i + 1] == '\\')
				printf("\\");
			else if (str[i + 1] == 'c')
				return (1);
			else
				printf("\\%c", str[i + 1]);
		}
		else
			printf("%c", str[i]);
		i++;
	}
	return (0);
}

int	info_flags(char *arg, int *n_flag, int *e_flag)
{
	int	i;

	i = 1;
	*n_flag = 0;
	*e_flag = 0;
	if (arg[0] != '-')
		return (0);
	while (arg[i])
	{
		if (arg[i] == 'n')
			*n_flag = 1;
		else if (arg[i] == 'e')
			*e_flag = 1;
		else if (arg[i] == 'E')
			*e_flag = 0;
		else
			return (0);
		i++;    
	}
	return (1);
}

int	builtin_echo(t_shell *shell)
{
	int	i;
	int	n_flag;
	int	e_flag;

	i = 1;
	while (info_flags(shell->cmds->argv[1], &n_flag, &e_flag) && shell->cmds->argv[i]) //flagleri kontrol ediyoruz, varsa onları atlayarak sonraki argümanlara geçiyoruz.
		i++;
	while (shell->cmds->argv[i])
	{
		if (e_flag)//-e flagi varsa argümanlarda bulunan escape karakterlerini işleyerek yazdırıyoruz.
		{
			if (print_with_escapes(shell->cmds->argv[i], 0))
				return (0);
		}
		else
			printf("%s", shell->cmds->argv[i]);
		if (shell->cmds->argv[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (0);
}