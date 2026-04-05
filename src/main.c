#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_shell *shell; //zurnanın zırt dediği yerdirr
    t_cmd *cmd; //parser sonucu alıp üzerinde execute edeceğim yapı
    char *input; //alacağımız komutlar

    shell = init_shell(shell, envp); // t_shell burada doğuyor shell bebek
    while (1)
    {
        input = readline("minishell");
        if(input == NULL) //crtl+d de redline NULL döner
        {
            write(2, "\nexit\n", 6);
            rl_clear_history();
            break ;
        }
        if (input != '\0') //bash de enter a basınca boş inputu kaydetmiyor soo biz de kaydetmiyoruz
            add_history(input);
        cmd = start_parser(input, shell); 
        if (cmd)
            execute(cmd, shell);
        free(input);
    }
    return (0);
}