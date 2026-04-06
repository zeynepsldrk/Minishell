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
    else if (is_builtin(cmd->argv[0])) //tek cmd varsa ve bu cmd bir built-in komutsa
        execute_builtin(shell); //built-in komutları çalıştırır. İçeride redirect işlemleri kontrolü yapmayı unutma
    else
        execute_external(shell); //external komutları çalıştırır. İçeride redirect işlemleri kontrolü yapmayı unutma
    
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

t_shell *init_shell(t_shell *shell, char **envp)
{ 
    shell->env_list = NULL;
    shell->env = envp;
    shell->exit_value = 0;
    shell->tokens = NULL;
    shell->cmds = NULL;
    return (shell);
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