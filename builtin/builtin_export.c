/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <asay@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 20:15:57 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/20 17:05:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_append(t_shell *shell, char *key, char *new_val)
{
	t_env_node  *node;
	char		*joined;

	node = shell->env_list;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			if (node->value && new_val)
			{
				joined = ft_strjoin(node->value, new_val); // mevcut + yeni
				free(node->value);
				node->value = joined;
			}
			else if (new_val)
				node->value = ft_strdup(new_val);
			return ;
		}
		node = node->next;
	}
	create_new_node(shell, key, new_val);
}

void execute_export(t_shell *shell, char *key, char *value, int i)
{
	if (ft_isappend(shell->cmds->argv[i]))
		handle_append(shell, key, value);   // += durumu
	else
	{
		if (is_key_inside(key, shell->env_list))
			update_env_node(key, value, shell);
		else
			create_new_node(shell, key, value);
	}
}

void only_export_command(t_env_node *env_list)
{
	t_env_node	*printable_copy;
	t_env_node	*head;

	printable_copy = ft_copy_env_list(env_list);
	head = printable_copy;
	ft_bubble_sort(&printable_copy, 1);
	while (printable_copy)
	{
		printf("declare -x %s", printable_copy->key);
		if (printable_copy->value) //env de görünmeyen value değerleri NULL olan değişkenleri de export ile yazdırabilmek için(= olmadan yazılıyorlar)
			printf("=\"%s\"", printable_copy->value); //value tırnak içinde yazılmalı
		printf("\n");
		printable_copy = printable_copy->next;
	}
	// free the copied list
	while (head)
	{
		printable_copy = head->next;
		free(head->key);
		free(head->value);
		free(head);
		head = printable_copy;
	}
}

int	process_export_arg(t_shell *shell, int i)
{
	char	*key;
	char	*value;

	if (is_valid_identifier(shell->cmds->argv[i]))
		return (write(2, "export: not a valid identifier\n", 31), 1);
	key = find_key_or_value(shell->cmds->argv[i], 0, KEY);
	value = find_key_or_value(shell->cmds->argv[i], 0, VALUE);
	if (!key)
	{
		free(value);
		return (1);
	}
	execute_export(shell, key, value, i);
	free(key);
	free(value);
	return (0);
}

int builtin_export(t_shell *shell, int in_pipe)
{
	int i;
	int ret;

	i = 1;
	(void)in_pipe;
	ret = 0;
	if (!shell->cmds->argv[1])
		return (only_export_command(shell->env_list), 0);
	while (shell->cmds->argv[i])
	{
		if (process_export_arg(shell, i))
			ret = 1;
		i++;
	}
	return (ret);
}
