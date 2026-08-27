/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:25:09 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/21 20:41:50 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

static void	execute_input(t_shell *shell)
{
	shell->cmds = start_parser(shell->input, shell);
	if (shell->cmds)
	{
		shell->pipes.command_count = ft_command_count(shell->cmds);
		shell->pipes.pipe_count = shell->pipes.command_count - 1;
		start_execute(shell);
		ft_free_cmd_list(shell->cmds);
		shell->cmds = NULL;
	}
	free(shell->input);
}

void	lets_start_shell(t_shell *shell)
{
	while (1)
	{
		if (g_signal == SIGINT)
		{
			shell->exit_value = 130;
			g_signal = 0;
		}
		shell->input = readline("minishell> ");
		if (shell->input == NULL)
			return (write(2, "exit\n", 5), rl_clear_history());
		if (*shell->input != '\0')
			add_history(shell->input);
		execute_input(shell);
	}
}

void	works_ctrl_c(int signal)
{
	g_signal = signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_forced_update_display();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argc;
	(void)argv;
	signal(SIGINT, works_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (1);
	if (!init_shell(shell, envp))
	{
		write(2, "Initialization failed\n", 22);
		free(shell);
		return (1);
	}
	lets_start_shell(shell);
	free_sh(shell);
	return (shell->exit_value);
}
