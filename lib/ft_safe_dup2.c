/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_dup2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 17:24:59 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 17:25:25 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_safe_dup2(int fd, int file_no)
{
	if (fd < 0 || file_no < 0)
	{
		write(2, "dup2: invalid fd\n", 17);
		return (-1);
	}
	if (dup2(fd, file_no) == -1)
	{
		perror("dup2");
		return (-1);
	}
	return (0);
}
