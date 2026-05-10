/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:04:17 by asay              #+#    #+#             */
/*   Updated: 2026/04/29 21:17:44 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalnum(char c)
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) 
        || (c >= 48 && c <= 57))
		return (1);
	return (0);
}

int	ft_strncmp(char *str1, char *str2, int n)
{
	int	i;

	if (n == 0)
		return (0);
	i = 0;
	while ((str1[i] || str2[i]) && i < n - 1 && str1[i] == str2[i])
		i++;
    return (str1[i] - str2[i]);
}
