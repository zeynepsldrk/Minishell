#include "minishell.h"

int builtin_cd(t_shell *shell);
int builtin_echo(t_shell *shell);



void execute_builtin_anyway(char *cmd, t_shell *shell) //cmd hem tek node hem de builtin ise zaten buraya gelmişizdir.
//Burada redir olup olmaması fark etmeksizin 
{

}

int is_builtin(char *cmd, t_shell *shell)
{
    if (!cmd)
    {
        shell->exit_value = 127;
        return (0);
    }
    if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "echo") == 0)
        return (1);
    else if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "pwd") == 0)
        return (1);
    else if (strcmp(cmd, "export") == 0 || strcmp(cmd, "unset") == 0)
        return (1);
    else if (strcmp(cmd, "env") == 0)
        return (1);
    return (0);
}
