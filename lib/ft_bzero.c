/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:23:35 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/29 18:23:36 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_bzero(void *p, int num)
{
	int i;
	unsigned char *ptr;

	i = 0;
	ptr = (unsigned char *)p;
	while (i < num)
	{
		ptr[i] = '\0';
		i++;
	}
}
