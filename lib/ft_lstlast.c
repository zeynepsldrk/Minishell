#include "minishell.h"

t_env_node	*ft_lstlast(t_env_node *env_list)
{
	if (!env_list)
		return (NULL);
	while (env_list->next)
		env_list = env_list->next;
	return (env_list);
}
