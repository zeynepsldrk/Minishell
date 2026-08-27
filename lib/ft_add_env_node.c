/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_env_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:23:13 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/21 17:32:22 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env_node(t_env_node **env_list, t_env_node *new_node)
{
	t_env_node	*last;

	if (!env_list || !new_node)
		return ;
	if (*env_list == NULL)
	{
		*env_list = new_node;
		return ;
	}
	last = ft_lstlast(*env_list);
	last->next = new_node;
}
