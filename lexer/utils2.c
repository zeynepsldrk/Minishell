/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:04:17 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 19:02:02 by asay             ###   ########.fr       */
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

int	numlen(long nbr)
{
	long	len;

	len = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
		len++;
	while (nbr)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

static void	fill_digits(char *res, long len, int nbr)
{
	if (nbr == 0)
	{
		res[0] = '0';
		res[1] = '\0';
		return ;
	}
	while (nbr)
	{
		res[len] = (nbr % 10) + '0';
		len--;
		nbr /= 10;
	}
}

char	*ft_itoa(int nbr)
{
	char	*res;
	long	len;

	len = numlen(nbr);
	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (0);
	res[len] = '\0';
	len--;
	if (nbr < 0)
	{
		res[0] = '-';
		nbr *= -1;
	}
	fill_digits(res, len, nbr);
	return (res);
}
