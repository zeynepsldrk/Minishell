/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:25:13 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/21 18:08:55 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_redirect_out(t_redirect *redir)
{
	int	fd;

	fd = open(redir->target, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (ft_redir_error(fd))
		return (1);
	if (ft_safe_dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_redirect_in(t_redirect *redir)
{
	int	fd;

	fd = open(redir->target, O_RDONLY);
	if (ft_redir_error(fd))
		return (1);
	if (ft_safe_dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_append(t_redirect *redir)
{
	int	fd;

	fd = open(redir->target, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (ft_redir_error(fd))
		return (1);
	if (ft_safe_dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_redir(t_redirect *redir)
{
	while (redir)
	{
		if (redir->type == REDIRECT_IN)
		{
			if (apply_redirect_in(redir))
				return (1);
		}
		else if (redir->type == REDIRECT_OUT)
		{
			if (apply_redirect_out(redir))
				return (1);
		}
		else if (redir->type == HEREDOC)
		{
			if (apply_heredoc(redir))
				return (1);
		}
		else if (redir->type == APPEND)
		{
			if (apply_append(redir))
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}
