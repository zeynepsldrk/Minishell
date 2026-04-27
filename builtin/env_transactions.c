#include "minishell.h"

void	update_env_node(char *key, char *value, t_shell *shell) //sadece value yu günceller
{
	int			i;//shell içinde ki env_list in içerisinde gezineceğim ve key olarak verilen değeri gödüğümüzde onu value ile değiştireceğiz.
	t_env_node	*temp;

	i = 0;
	temp = shell->env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
		{
			if (temp->value)
				free(temp->value);
			temp->value = ft_strdup(value);
            return ;
		}
		temp = temp->next;
	}
}

void create_new_node(t_shell *shell, char *key, char *value)
{
    t_env_node *new_node;

    new_node = (t_env_node *)malloc(sizeof(t_env_node));
    if (!new_node)
        return ;
    new_node->key = safe_strdup(key);
    new_node->value = safe_strdup(value);
    new_node->next = NULL;
    if (!new_node->key)  // key her zaman olmalı, value NULL olabilir
    {
        free(new_node->value);
        free(new_node);
        return ;
    }
    add_env_node(&shell->env_list, new_node);
}

void delete_env_node(t_env_node **env_list, char *key, char *value)
{
    int i;
    t_env_node *temp;

    i = 0;
    while (*env_list)
    {
        if (ft_strcmp((*env_list)->key, key) == 0)
        {
            temp = *env_list;
            *env_list = (*env_list)->next;
            free(temp->key);
            free(temp->value);
            free(temp);
            return ;
        }
        env_list = &(*env_list)->next;
    }
}

void print_env_list(t_env_node *env_list)
{
    while (env_list)
    {
        printf("%s=%s\n", env_list->key, env_list->value);
        env_list = env_list->next;
    }
}
