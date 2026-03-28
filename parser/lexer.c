#include "minishell.h"

int	nonspace_index(char *str, int ind)
{
	while(str[ind] != ' ' && str[ind] != '\t')
		ind++;
	return ind;
}

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if(!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->ne = NULL;
	return (token);
}

void	get_value(char *str, int i)
{
	int j;
	t_token *token;

	//mem allocate for token
	j = nonspace_index(str, i);
	while (i < j)
	{
		if (str[i] == '>')
		{
			if(str[i+1] == '>')
				token->type = APPEND;
			else
				token->type = REDIRECT_OUT;
		}
		if (str[i] == '<')
		{
			if(str[i+1] == '<')
				token->type = HEREDOC;
			else
				token->type = REDIRECT_IN;
		}
		i++;
	}
}
