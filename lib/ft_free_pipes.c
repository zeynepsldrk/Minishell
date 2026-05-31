#include "minishell.h"

void	free_pipes(int **fd, int count)
{
	while (--count >= 0)
	{
		close(fd[count][0]);
		close(fd[count][1]);
		free(fd[count]);
	}
	free(fd);
}
