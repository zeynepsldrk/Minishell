#include "minishell.h"

int ft_redir_error(int fd)
{
    if (fd < 0)
    {
        perror("open");
        return (1);
    }
    return (0);
}
