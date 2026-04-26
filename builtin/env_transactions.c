#include "minishell.h"

void	update_env_node(char *key, char *value, t_shell *shell) //sadece value yu günceller
{
	int			i;//shell içinde ki env_list in içerisinde gezineceğim ve key olarak verilen değeri gödüğümüzde onu value ile değiştireceğiz.
	t_env_node	*temp;

	i = 0;
	temp = shell->env_list;
	while (temp)
	{
		if (str_cmp(temp->key, key) == 0)
		{
			if (temp->value)
				free(temp->value);
			temp->value = str_dup(value);
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
    new_node->key = str_dup(key);
    new_node->value = str_dup(value);
    new_node->next = NULL;
    if (!new_node->key || (value && !new_node->value))
    {
        free(new_node->key);
        free(new_node->value);
        free(new_node);
        return ;
    }
    add_env_node(&shell->env_list, new_node);
}

void delete_env_node(t_env_node **env_list, char *key, char *value)
{
    
}

void print_env_list(t_env_node *env_list)
{

}
