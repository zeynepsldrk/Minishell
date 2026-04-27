#include "minishell.h"

char	*ft_strdup(const char *s)
{
	int		len;
	size_t	i;
	char	*a;

	len = ft_strlen(s);
	i = 0;
	if (!s)
		return (NULL);
	a = (char *)malloc((len + 1) * (sizeof(char)));
	if (!a)
		return (NULL);
	while (s[i])
	{
		a[i] = s[i];
		i++;
	}
	a[i] = '\0';
	return (a);
}
