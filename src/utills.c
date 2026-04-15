#include "minishell.h"

int redir_error(int fd)
{
    if (fd < 0)
    {
        perror("open");
        return (1);
    }
    return (0);
}

int str_len(char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

int	str_cmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}