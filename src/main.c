#include "minishell.h"

void start_execute(t_shell *shell)
{
    
}

void lets_start_shell(t_shell *shell)
{
    while (1)
    {
        shell->input = readline("minishell");
        if(shell->input == NULL) //crtl+d de readline NULL döner
        {
            write(2, "\nexit\n", 6);
            rl_clear_history();
            break ;
        }
        if (shell->input != '\0') //bash de enter a basınca boş inputu kaydetmiyor soo biz de kaydetmiyoruz
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
    t_shell *shell; //zurnanın zırt dediği yerdirr
    (void)argc;
    (void)argv;

    shell = malloc(sizeof(t_shell));
    if (!shell)
        return (NULL); 
    if (!init_shell(shell, envp))
    {
        write(2, "Initialization failed\n", 22);
        free(shell);
        return (NULL);
    }
    lets_start_shell(shell);
    free(shell);
    return (0);
}