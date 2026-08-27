/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 17:37:18 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 17:37:18 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_node(t_env_node *env)
{
	t_env_node	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	free_sh(t_shell *shell)
{
	if (shell->input)
		free(shell->input);
	free_str(shell->env);
	free_env_node(shell->env_list);
	free_tokens(shell->tokens);
	free_commands(shell->cmds);
	free(shell);
}
