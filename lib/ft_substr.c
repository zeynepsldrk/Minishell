/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <asay@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:24:11 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/07 20:50:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr(char *s,int start, int len)
{
	char	*subs;
	int	size;

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
