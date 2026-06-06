/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 20:19:36 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/06 20:33:06 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	here_doc_signal(t_redirect *redir, int status)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_signal = SIGINT;
		close(redir->heredoc_fd[0]);
		return (1);
	}
	return (0);
}

void	here_doc(t_redirect *redir)
{
	char *line;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, redir->target_file) == 0)
		{
			free(line);
			break;
		}
		write(redir->heredoc_fd[1], line, ft_strlen(line));
		write(redir->heredoc_fd[1], "\n", 1);
		free(line);
	}
	close(redir->heredoc_fd[1]);
}

int apply_heredoc(t_redirect *redir)
{
	int pid;
	int status;

	if (pipe(redir->heredoc_fd) == -1)
		return (1);
	pid = fork();
	if (pid == 0)
	{
		close(redir->heredoc_fd[0]);
		here_doc(redir);
		exit(0);
	}
	close(redir->heredoc_fd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, works_ctrl_c);
	if (here_doc_signal(redir, status) == 1)
		return (1);
	if (ft_safe_dup2(redir->heredoc_fd[0], STDIN_FILENO) == -1)
	{
		close(redir->heredoc_fd[0]);
		return (1);
	}
	close(redir->heredoc_fd[0]);
	return (0);
}
