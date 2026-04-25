/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:25:09 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/25 15:15:56 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd *start_parser(char *input, t_shell *shell)
{
    (void)input;
    (void)shell;
    return (NULL);
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
    else if (is_builtin(cmd->argv[0], shell)) //cmd tek ise builtin mi diye kontrol eder
        execute_builtin(cmd->argv[0], shell, 0); //forklamadan built-in komutları çalıştırır. İçeride redirect işlemleri kontrolü yapmayı unutma
    else
        execute_external(shell); //fork lazımdıır,external komutları çalıştırır. İçeride redirect işlemleri kontrolü yapmayı unutma
    //!!sondaki else bir şekilde azaltılabilir mi diye düşünüyorum, çünkü cmd tek ise ve builtin değilse zaten external komut oluyor, yani tek if ile de halledilebilir gibi geliyor bana.!!
    //externallerin nasıl çalıştırıldığını anlayınca tekrar değerlendirilecektir bu durum.!!
}

void lets_start_shell(t_shell *shell)
{
    while (1)
    {
        shell->input = readline("minishell> ");
        if(shell->input == NULL) //crtl+d de readline NULL döner
        {
            write(2, "exit\n", 5);
            rl_clear_history();
            break ;
        }
        if (*shell->input != '\0') //bash de enter a basınca boş inputu kaydetmiyor soo biz de kaydetmiyoruz
            add_history(shell->input);
        shell->cmds = start_parser(shell->input, shell); 
        if (shell->cmds)
            start_execute(shell);
        free(shell->input);
    }
}

int main(int argc, char **argv, char **envp)
{
    t_shell *shell;
    (void)argc;
    (void)argv;

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
