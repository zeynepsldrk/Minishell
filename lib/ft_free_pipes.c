/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 17:20:27 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 17:20:28 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_pipes(int **fd, int count)
{
	while (--count >= 0)
	{
		close(fd[count][0]);
		close(fd[count][1]);
		free(fd[count]);
	}
	free(fd);
}
