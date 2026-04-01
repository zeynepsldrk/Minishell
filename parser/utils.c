/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 18:06:47 by asay              #+#    #+#             */
/*   Updated: 2026/04/01 18:56:46 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
int ft_strlen(char *str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *str)
{
	char	*dupe;
	size_t	i;

	i = 0;
	dupe = (char *)malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!dupe)
		return (NULL);
	while (str[i])
	{
		dupe[i] = str[i];
		i++;
	}
	dupe[i] = '\0';
	return (dupe);
}

char *trim(char *str)
{
    int i;
    int j;
    int k;
    char *dup;
    
    i = 0;
    j = ft_strlen(str) - 1;
    k = 0;
    dup = malloc((ft_strlen(str) + 1));
    if(!dup)    
        return NULL;
    while((str[i] == ' ' || str[i] == '\t') && str[i])
        i++;
    while((str[j] == ' ' || str[j] == '\t' && j > 0))
        j--;
    while(i < j)
        dup[k++] = str[i++];
    dup[k] = '\0';
    return dup;
}

char *ft_substr(char *str, int start, int len)
{
    int i;
    char *sub;

    sub = malloc((len + 1));
    if(!sub)
        return 0;
    i = 0;
    while(str[start] && i < len)
        sub[i++] = str[start++];
    sub[i] = '\0';
    return (sub);
}

// int	is_quote_open(char *str)
// {
// 	int	in_double;
// 	int in_single;
// 	int	i;

// 	in_double = 0;
// 	in_single = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == 34 && !in_single)
// 			in_double = !in_double;
// 		else if (str[i] == 39 && !in_double)
// 			in_single = !in_single;
// 		i++;
// 	}
// 	return (in_double || in_single);
// }

