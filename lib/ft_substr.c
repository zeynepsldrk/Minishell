/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:24:11 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/29 18:26:51 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*subs;
	size_t	size;

	if (!s)
	{
		return (NULL);
	}
	if (start >= ft_strlen(s))
	{
		return (ft_strdup(""));
	}
	size = ft_strlen(s + start);
	if (size < len)
	{
		len = size;
	}
	subs = (char *)malloc(sizeof(char) * (len + 1));
	if (!subs)
	{
		return (NULL);
	}
	ft_strlcpy(subs, s + start, len + 1);
	return (subs);
}
