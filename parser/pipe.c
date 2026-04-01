/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 20:55:05 by asay              #+#    #+#             */
/*   Updated: 2026/04/01 19:10:36 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int pipe_ctrl(char *str, int i)
{
	int in_s;
	int in_d;

	in_s = 0;
	in_d = 0;
	while(str[i])
	{
		if (str[i] == 34 && !in_s)
			in_d = !in_d;
		if (str[i] == 39 && !in_d)
			in_s = !in_s;
		if (str[i] == '|' && (in_s == 0 && in_d == 0))
			return i;
		i++;
	}
}

int pipe_count(char *str)
{
    //gerçek pipe'ları sayıp ona göre input output sayısı belirlenecek.
}
