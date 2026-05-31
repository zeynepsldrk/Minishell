#include "minishell.h"

static t_env_node *ft_new_env_node(t_env_node *src)
{
	t_env_node *new_node;

	new_node = malloc(sizeof(t_env_node));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(src->key);
	new_node->value = NULL;
	if (src->value)
		new_node->value = ft_strdup(src->value);
	new_node->next = NULL;
	return (new_node);
}

t_env_node *ft_copy_env_list(t_env_node *env_list)
{
	t_env_node *head;
	t_env_node *prev;
	t_env_node *new_node;

	head = NULL;
	prev = NULL;
	while (env_list)
	{
		new_node = ft_new_env_node(env_list);
		if (!new_node)
			return (head);
		if (prev)
			prev->next = new_node;
		else
			head = new_node;
		prev = new_node;
		env_list = env_list->next;
	}
	return (head);
}