/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:23:26 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/29 18:23:27 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void safe_transform(char *str, int *i, long *result, int *sign)
{
    while (str[*i] >= '0' && str[*i] <= '9')
    {
        if (*result > (LONG_MAX - (str[*i] - '0')) / 10)
        {
            errno = ERANGE;
            return;
        }
        *result = *result * 10 + (str[*i] - '0');
        (*i)++;
    }
}

long	ft_atol(const char *str)
{
	int		i;
	int		sign;
	long	result;

	sign = 1;
	result = 0;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
		{
			sign = -1;
		}
		i++;
	}
    errno = 0; // errno'yu sıfırla, safe_transform sırasında sayısal olmayan karakterler varsa errno'yu ERANGE yapacak
    //errno zaten kütüphane içinde tanımlı geliyor
    safe_transform(str, &i, &result, &sign); // sayısal olmayan karakterler varsa errno'yu ERANGE yapar
	return (sign * result);
}