/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:24:01 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/29 18:26:45 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(const char *s)
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

char *safe_strdup(char *s)
{
    if (!s)
        return (NULL);
    return (ft_strdup(s));
}
