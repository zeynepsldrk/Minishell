#include "minishell.h"

t_builtin *init_builtins(t_builtin *builtins)
{
    builtins = malloc(sizeof(t_builtin) * 7);
    if (!builtins)
        return (NULL);
    builtins[0].name = "cd";
    builtins[0].func = builtin_cd;
    builtins[1].name = "echo";
    builtins[1].func = builtin_echo;
    builtins[2].name = "exit";
    builtins[2].func = builtin_exit;
    builtins[3].name = "pwd";
    builtins[3].func = builtin_pwd;
    builtins[4].name = "export";
    builtins[4].func = builtin_export;
    builtins[5].name = "unset";
    builtins[5].func = builtin_unset;
    builtins[6].name = "env";
    builtins[6].func = builtin_env;
    return (builtins);
}

t_shell *init_shell(t_shell *shell, char **envp)
{ 
    shell->env_list = NULL;
    shell->env = envp;
    shell->exit_value = 0;
    shell->tokens = NULL;
    shell->cmds = NULL;
    shell->builtins = init_builtins(shell->builtins); //header dosyasına eklediğim struct ın 
    //içini builtin fonksiyonlarının adları ve çalıştıracağı fonk. atadım. İndex kullanarak fonksiyonları çağırmak istediğimde kolaylık sağlayacak.
    return (shell);
}