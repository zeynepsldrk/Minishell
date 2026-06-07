/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <asay@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:24:01 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/07 21:53:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(char *s)
{
	int		len;
	size_t	i;
	char	*a;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	i = 0;
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

char *ft_safe_strdup(char *s)
{
    if (!s)
        return (NULL);
    return (ft_strdup(s));
}
