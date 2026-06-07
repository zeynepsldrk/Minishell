/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_indentifier.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:26:15 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/29 18:26:57 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(char *arg)
{
	int j;

	j = 0;
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
		return (1);
	while (arg[j] && arg[j] != '=')
	{
		if (arg[j] == '+' && arg[j + 1] == '=')
			return (0);
		if (!(ft_isalnum(arg[j]) || arg[j] == '_'))
			return (1);
		j++;
	}
	return (0);
}
