/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <asay@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:25:09 by zedurak           #+#    #+#             */
/*   Updated: 2026/06/19 23:34:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "getnextline/get_next_line.h" //added for tester, will be deleted after

int	g_signal;

t_cmd *start_parser(char *input, t_shell *shell)
{
    char *trimmed;

    trimmed = trim(input);
    if (!trimmed || trimmed[0] == '\0')
    {
        free(trimmed);
        return (NULL);
    }
    lexer(shell, trimmed);
    expander(shell);
	rm_empty_token(shell);
	join_tokens(shell);
    parser(shell);
    free(trimmed);
    return (shell->cmds);
}

void start_execute(t_shell *shell)
{
	t_cmd *cmd;

	cmd = shell->cmds;
	if (!cmd || !cmd->argv)
		return ;
	/*Burada yapmaya çalıştığım pipe kullanımına göre ayırmak ve kullanılmadığı takdirde builtin ya da external
	olarak çalıştırılması, ek olarak içeride redirect işlemleri kontrolü de yapılmalıdır.*/
	if (cmd->next != NULL) //birden fazla cmd varsa pipe var demektir
		pipe_working(shell); //cmd listesini dolaşarak pipe işlemlerini yapar
		//en son child içinde en son child in exit kodunu shell->exit_value a yazmak lazım
		//Bu pipe'ta 3 komut var. Bash'in kuralı: en sondaki komutun exit kodu önemli. ls hata verse bile, wc -l başarılıysa $? = 0 olur.
	else if (is_builtin(cmd->argv[0], shell)) //cmd tek ise builtin mi diye kontrol eder
		execute_builtin(cmd->argv[0], shell, 0, 0); //forklamadan built-in komutları çalıştırır. İçeride redirect işlemleri kontrolü yapmayı unutma
		//builtinler doğru ise 0 dönmeli hatalı ise 1 dönmeli
	else
		execute_external(shell, 0); //fork lazımdıır,external komutları çalıştırır. İçeride redirect işlemleri kontrolü yapmayı unutma
	//!!sondaki else bir şekilde azaltılabilir mi diye düşünüyorum, çünkü cmd tek ise ve builtin değilse zaten external komut oluyor, yani tek if ile de halledilebilir gibi geliyor bana.!!
	//externallerin nasıl çalıştırıldığını anlayınca tekrar değerlendirilecektir bu durum.!!
	//execute_external içinde waitpid'den gelen kodu
	//shell->exit_value'a yazman lazım
}

static char    *trim_newline(char *line) //added for tester, will be deleted after
{
    int     len;
    char    *result;

    len = 0;
    while (line[len] && line[len] != '\n')
        len++;
    result = malloc(len + 1);
    if (!result)
        return (NULL);
    result[len] = '\0';
    while (len--)
        result[len] = line[len];
    free(line);
    return (result);
}
void lets_start_shell(t_shell *shell)
{
	char	*line;

	while (1)
	{
		if (g_signal == SIGINT)
		{
			shell->exit_value = 130;
			g_signal = 0;
		}
		// change this part for tester, will be changed after
		if (isatty(fileno(stdin)))
			shell->input = readline("minishell> ");
		else
		{
			line = get_next_line(fileno(stdin));
			if (!line)
				shell->input = NULL;
			else
				shell->input = trim_newline(line);
		}
		if (shell->input == NULL)
		{
			//write(2, "exit\n", 5); // commented for tester, will be added after
			rl_clear_history();
			break ;
		}
		if (*shell->input != '\0')
			add_history(shell->input);
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
}

void works_ctrl_c(int signal)
{
	g_signal = signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_forced_update_display();
}

int main(int argc, char **argv, char **envp)
{
	t_shell *shell;
	(void)argc;
	(void)argv;

	signal(SIGINT, works_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (1); 
	if (!init_shell(shell, envp)) //default değerlerle dolduruldu
	{
		write(2, "Initialization failed\n", 22);
		free(shell);
		return (1);
	}
	lets_start_shell(shell); //repl döngüsü başlatıldı
	free(shell);
	return (0);
}
