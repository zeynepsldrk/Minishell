/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_external_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 17:30:29 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/21 14:08:44 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_the_path(char *path, char *cmd)
{
	char	**splited_path;
	char	*full_path;
	int		i;

	splited_path = ft_split(path, ':');
	i = 0;
	while (splited_path[i])
	{
		full_path = ft_strjoin(splited_path[i], "/");
		full_path = ft_join_and_free(full_path, cmd);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			ft_free_split(splited_path);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_split(splited_path);
	return (NULL);
}

void	print_path_error(t_shell *shell, char *cmd, char *msg, int exit_code)
{
	shell->exit_value = exit_code;
	if (cmd && *cmd)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": ", 2);
	}
	if (msg)
		write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

int	is_path_okey(t_shell *shell, char *path)
{
	struct stat	st;

	if (access(path, F_OK)) //dosya disk de var mı?
	{
		print_path_error(shell, path, "command not found", 127);
		return (1);
	}
	stat(path, &st);
	if (S_ISDIR(st.st_mode)) // Klasör mü, dosya mı?
	{
		print_path_error(shell, path, "is a directory", 126);
		return (1);
	}
	if (access(path, X_OK)) //çalıştırılabilir mi?
	{
		print_path_error(shell, path, "Permission denied", 126);
		return (1);
	}
	return (0);
}

int	is_absolute_path(char *path)
{
	if (!path || !path[0])
		return (1);
	if (path[0] == '/')
		return (0);
	if (path[0] == '.')
	{
		if (path[1] == '/' || (path[1] == '.' && path[2] == '/'))
			return (0);
	}
	return (1);
}

char	*find_command_path(t_shell *shell)
{
	char	*path;

	if (is_absolute_path(shell->cmds->argv[0]) == 0)
	{
		if (is_path_okey(shell, shell->cmds->argv[0]) == 0)
			return (ft_strdup(shell->cmds->argv[0]));
		return (NULL);
	}
	path = my_little_getenv(shell->env_list, "PATH");
	if (!path)
		return (NULL);
	path = check_the_path(path, shell->cmds->argv[0]);
	if (path && (is_path_okey(shell, path) == 0))
		return (path);
	free(path);
	return (NULL);
}
