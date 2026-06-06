/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_transactions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 13:01:23 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/06 18:39:29 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_env_node(char *key, char *value, t_shell *shell) //sadece value yu günceller
{
	//shell içinde ki env_list in içerisinde gezineceğim ve key olarak verilen değeri gödüğümüzde onu value ile değiştireceğiz.
	t_env_node	*temp;

	temp = shell->env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
		{
			if (temp->value)
				free(temp->value);
			temp->value = ft_strdup(value);
			return (0);
		}
		temp = temp->next;
	}
	return (1);
}



int create_new_node(t_shell *shell, char *key, char *value)
{
	t_env_node *new_node;

	new_node = (t_env_node *)malloc(sizeof(t_env_node));
	if (!new_node)
		return (1);
	new_node->key = ft_safe_strdup(key);
	new_node->value = ft_safe_strdup(value);
	new_node->next = NULL;
	if (!new_node->key)  // key her zaman olmalı, value NULL olabilir
	{
		free(new_node->value);
		free(new_node);
		return (1);
	}
	add_env_node(&shell->env_list, new_node);
	return (0);
}

int delete_env_node(t_env_node **env_list, char *key)
{
	t_env_node *temp;

	while (*env_list)
	{
		if (ft_strcmp((*env_list)->key, key) == 0)
		{
			temp = *env_list;
			*env_list = (*env_list)->next;
			free(temp->key);
			free(temp->value);
			free(temp);
			return (0);
		}
		env_list = &(*env_list)->next;
	}
	return (1);
}

int print_env_list(t_env_node *env_list)
{
	while (env_list)
	{
		if (env_list->value != NULL)
			printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
	return (0);
}
