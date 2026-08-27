/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 20:00:25 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 20:00:33 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*start_parser(char *input, t_shell *shell)
{
	char	*trimmed;

	trimmed = trim(input);
	if (!trimmed || trimmed[0] == '\0')
	{
		free(trimmed);
		return (NULL);
	}
	lexer(shell, trimmed);
	expander(shell);
	rm_empty_token(shell);
	join_tokens(shell);
	parser(shell);
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	free(trimmed);
	return (shell->cmds);
}

void	start_execute(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = shell->cmds;
	if (!cmd || !cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
		return ;
	if (cmd->next != NULL)
		pipe_working(shell);
	else if (is_builtin(cmd->argv[0], shell))
		execute_builtin(cmd->argv[0], shell, 0, 0);
	else
		execute_external(shell, 0);
}
