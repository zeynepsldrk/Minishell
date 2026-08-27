/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 17:55:04 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 17:56:37 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim(char *str)
{
	int		i;
	int		j;
	int		k;
	char	*dup;

	i = 0;
	j = ft_strlen(str) - 1;
	k = 0;
	dup = malloc((ft_strlen(str) + 1));
	if (!dup)
		return (NULL);
	while ((str[i] == ' ' || str[i] == '\t') && str[i])
		i++;
	while ((str[j] == ' ' || str[j] == '\t') && j > 0)
		j--;
	while (i <= j)
		dup[k++] = str[i++];
	dup[k] = '\0';
	return (dup);
}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*b2;
	unsigned char	c2;
	size_t			i;

	i = 0;
	b2 = (unsigned char *)b;
	c2 = (unsigned char)c;
	while (i < len)
	{
		b2[i] = c2;
		i++;
	}
	return (b);
}
