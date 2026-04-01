/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:38:00 by asay              #+#    #+#             */
/*   Updated: 2026/04/01 18:29:39 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


t_token	*new_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if(!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

t_token_type	get_value(char *str, int *i)
{
	if (str[*i] == '>')
	{
		if(str[*i+1] == '>')
		{
			return(APPEND);
			(*i)++;
		}
		return(REDIRECT_OUT);
	}
	else if (str[*i] == '<')
	{
		if(str[*i+1] == '<')
		{
			return(HEREDOC);
			(*i)++;
		}
		return(REDIRECT_IN);
	}
	else if(str[*i] == '|')
		return(PIPE);
	else
		return(WORD);
	(*i)++;
}
