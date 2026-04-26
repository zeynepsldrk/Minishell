#include "minishell.h"

void	my_bzero(void *p, int num)
{
	int i;
	unsigned char *ptr;

	i = 0;
	ptr = (unsigned char *)p;
	while (i < num)
	{
		ptr[i] = '\0';
		i++;
	}
}

t_env_node	*ft_lstlast(t_env_node *env_list)
{
	if (!env_list)
		return (NULL);
	while (env_list->next)
		env_list = env_list->next;
	return (env_list);
}

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
