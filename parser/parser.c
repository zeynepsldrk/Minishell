/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 20:06:08 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 20:06:14 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	cmds_syntax_checker(t_shell *sh)
{
	t_token	*token;

	token = sh->tokens;
	while (token && token->type != PIPE)
	{
		if ((token->type == REDIRECT_IN || token->type == REDIRECT_OUT
				|| token->type == HEREDOC || token->type == APPEND)
			&& token->next && token->next->type == PIPE)
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
			sh->exit_value = 2;
			return (1);
		}
		token = token->next;
	}
	return (0);
}

t_cmd	*get_cmds(t_shell *sh)
{
	t_token	*token;
	t_cmd	*cmd;
	t_cmd	*head;

	token = sh->tokens;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	fill_cmd(cmd, token);
	head = cmd;
	while (token != NULL)
	{
		token = token->next;
		if (token == NULL)
			break ;
		if (token->type == PIPE)
			handle_pipe(&cmd, &token);
	}
	cmd->next = NULL;
	if (cmds_syntax_checker(sh))
		return (ft_free_cmd_list(head), NULL);
	return (head);
}

static t_redirect	*new_redirect(t_token *token)
{
	t_redirect	*rdr;

	rdr = malloc(sizeof(t_redirect));
	if (!rdr)
		return (NULL);
	rdr->type = token->type;
	rdr->target = ft_strdup(token->next->context);
	rdr->next = NULL;
	return (rdr);
}

t_redirect	*get_redirs(t_token *token)
{
	t_redirect	*rdr;
	t_redirect	*head;
	t_redirect	*pre;

	head = NULL;
	pre = NULL;
	while (token != NULL && token->type != PIPE)
	{
		if (token->type == REDIRECT_IN || token->type == REDIRECT_OUT
			|| token->type == HEREDOC || token->type == APPEND)
		{
			rdr = new_redirect(token);
			if (!rdr)
				return (NULL);
			if (head == NULL)
				head = rdr;
			else
				pre->next = rdr;
			pre = rdr;
		}
		token = token->next;
	}
	return (head);
}

void	parser(t_shell *sh)
{
	sh->cmds = get_cmds(sh);
}
