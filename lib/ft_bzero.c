#include "minishell.h"

void	ft_bzero(void *p, int num)
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
