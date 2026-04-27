#include "minishell.h"

void add_env_node(t_env_node **env_list, t_env_node *new_node)
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
	last-> next = new_node;
}
