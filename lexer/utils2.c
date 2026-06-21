/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:04:17 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 15:17:48 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int numlen(long nbr)
{
    long len = 0;

    if(nbr == 0)
        return 1;
    if(nbr < 0)
        len++;
    while(nbr)
    {
        nbr /= 10;
        len++;
    }
    return len;
}

char *ft_itoa(int nbr)
{
    char *res;
    long len = numlen(nbr);
    res = malloc((len + 1) * sizeof(char));
    if(!res)
        return 0;
    res[len] = '\0';
    len--;

    if(nbr < 0)
    {
        res[0] = '-';
        nbr *= -1;
    }
    if(nbr == 0)
    {
        res[0] = '0';
        res[1] = '\0';
        return res;
    }
    while(nbr)
    {
        res[len] = (nbr % 10) + '0';
        len--;
        nbr /= 10;
    }
    return res;
}
