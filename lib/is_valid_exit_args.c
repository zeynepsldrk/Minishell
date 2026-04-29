/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_exit_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:24:15 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/29 18:26:54 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_valid_exit_arg(const char *str)
{
    long    num;
    int     i;
    int     errno;

    i = 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (!str[i]) // sadece +/- girilmişse bu da sayısal değil
        return (0);
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0); // sayısal değil
        i++;
    }
    errno = 0;
    (void)ft_atol(str); // errno set etmek için kullanılıyor overflow olmuş mu vs. 
    if (errno == ERANGE) // taştı mı?
        return (0); // bash bunu "numeric argument required" sayar
    return (1);
}
