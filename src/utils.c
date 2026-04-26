/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:25:29 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/25 20:00:28 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int redir_error(int fd)
{
    if (fd < 0)
    {
        perror("open");
        return (1);
    }
    return (0);
}

int str_len(char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

int	str_cmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}
char	*str_dup(const char *s)
{
	int		len;
	size_t	i;
	char	*a;

	len = ft_strlen(s);
	i = 0;
	if (!s)
		return (NULL);
	a = (char *)malloc((len + 1) * (sizeof(char)));
	if (!a)
		return (NULL);
	while (s[i])
	{
		a[i] = s[i];
		i++;
	}
	a[i] = '\0';
	return (a);
}

int ft_isalpha(int c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int ft_isalnum(int c)
{
    return (ft_isalpha(c) || (c >= '0' && c <= '9'));
}
