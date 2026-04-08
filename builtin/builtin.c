#include "minishell.h"

int builtin_cd(t_shell *shell);
int builtin_echo(t_shell *shell);

void *apply_redir(t_redirect *redir, t_shell *shell)
{
    while (redir)
    {
        if(redir->type == REDIRECT_IN)
            apply_redirect_in(redir, shell);
        else if(redir->type == REDIRECT_OUT)
            apply_redirect_out(redir, shell);
        else if(redir->type == HEREDOC)
            apply_heredoc(redir, shell);
        else if(redir->type == APPEND)
            apply_append(redir, shell);
        redir = redir->next;
    }
}

void *execute_builtin(char *cmd, t_shell *shell) //cmd hem tek node hem de builtin ise zaten buraya gelmişizdir.
{                                           //Burada redir olup olmaması fark etmeksizin
    int backup_stdout;
    int backup_stdin;

    backup_stdout = dup(STDOUT_FILENO); //standart girdi/çıktıları önceden kaydediyoruz çünkü redirlere göre değerleri kaybolacak
    backup_stdin = dup(STDIN_FILENO);
    if(shell->cmds->redirects != NULL) //redir uygula -varsa tabii-
        apply_redir(shell->cmds->redirects, shell);
    while (shell->builtins) //komutu çalıştır
    {
        if (strcmp(cmd, shell->builtins->name) == 0)
        {
            shell->builtins->func(shell);
            break ;
        }
        shell->builtins++;
    }
    dup2(backup_stdout, STDOUT_FILENO); //fd leri eski haline getir
    dup2(backup_stdin, STDIN_FILENO);
    close(backup_stdout); // eski fdleri kapatma sebebimiz: hem eski fd nin hemde yeni fd nin
    close(backup_stdin);  //aynı anda açık kalması, sınırlı fd sayısına sahip olmamızdan kaynaklı boş yere çalışan fdlerin bulunmasına
    //ve iki fd nin de aynı yeri yönlendirmesine neden olur. Bunlardan kaynaklı ileride problemler çıkabilir.
    //Ayrıca dup yapınca bir dosya açmış oluyoruz, bu yüzden kapatmak gerekiyor.
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
