/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 20:38:09 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 20:39:49 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	word_count(t_token *token)
{
	int	count;

	count = 0;
	while (token != NULL && token->type != PIPE)
	{
		if (token->type == WORD)
			count++;
		if (token->type == REDIRECT_IN || token->type == REDIRECT_OUT
			|| token->type == HEREDOC || token->type == APPEND)
		{
			if (token->next == NULL)
				break ;
			token = token->next;
		}
		token = token->next;
	}
	return (count);
}

void	fill_cmd(t_cmd *cmd, t_token *token)
{
	cmd->argv = get_argv(token);
	cmd->redirects = get_redirs(token);
	cmd->argc = word_count(token);
	cmd->next = NULL;
}

int	handle_pipe(t_cmd **cmd, t_token **token)
{
	(*cmd)->next = malloc(sizeof(t_cmd));
	if (!(*cmd)->next)
		return (0);
	*token = (*token)->next;
	if (*token == NULL)
		return (0);
	*cmd = (*cmd)->next;
	fill_cmd(*cmd, *token);
	return (1);
}

char	**get_argv(t_token *token)
{
	char	**argv;
	int		i;

	argv = malloc(sizeof(char *) * (word_count(token) + 1));
	i = 0;
	if (!argv)
		return (NULL);
	while (token != NULL && token->type != PIPE)
	{
		if (token->type == WORD)
		{
			argv[i] = ft_strdup(token->context);
			i++;
		}
		if (token->type == REDIRECT_IN || token->type == REDIRECT_OUT
			|| token->type == HEREDOC || token->type == APPEND)
		{
			if (token->next == NULL)
				break ;
			token = token->next;
		}
		token = token->next;
	}
	argv[i] = NULL;
	return (argv);
}
