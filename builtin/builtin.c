#include "minishell.h"

int builtin_cd(t_shell *shell)
{

}

/*echo komutu için -n, -e ve -E flaglerinin kombinasyonlarında davranışlarını belirledim. */
int print_with_escapes(char *str, int i)
{
    while (str[i])
    {
        if (str[i] == '\\' && str[i + 1] != '\0')
        {
            if (str[i + 1] == 'n')
                printf("\n");
            else if (str[i + 1] == 't')
                printf("\t");
            else if (str[i + 1] == '\\')
                printf("\\");
            else if (str[i + 1] == 'c')
                return (1);
            else
                printf("\\%c", str[i + 1]);
        }
        else
            printf("%c", str[i]);
        i++
    }
    return (0);
}

int info_flags(char *arg, int *n_flag, int *e_flag, int *any_flag_here)
{
    int i;

    i = 1;
    *n_flag = 0;
    *e_flag = 0;
    *any_flag_here = 0;
    if (arg[0] != '-')
        return (0);
    while (arg[i])
    {
        if (arg[i] == 'n')
            *n_flag = 1;
        else if (arg[i] == 'e')
            *e_flag = 1;
        else if (arg[i] == 'E')
            *e_flag = 0;
        else
            return (0);
        i++;    
    }
    *any_flag_here = 1;
    return (1);
}

int builtin_echo(t_shell *shell)
{
    int i;
    int n_flag;
    int e_flag;
    int any_flag_here;

    i = 1;
    while (info_flags(shell->cmds->argv[1], &n_flag, &e_flag, &any_flag_here) && shell->cmds->argv[i]) //flagleri kontrol ediyoruz, varsa onları atlayarak sonraki argümanlara geçiyoruz.
        i++;
    while (shell->cmds->argv[i])
    {
        if (e_flag)//-e flagi varsa argümanlarda bulunan escape karakterlerini işleyerek yazdırıyoruz.
        {
            if (print_with_escapes(shell->cmds->argv[i], 0);)
                return (0);
        }
        else
            printf("%s", shell->cmds->argv[i]);
        if (shell->cmds->argv[i + 1])
            printf(" ");
        i++;
    }
    if (!n_flag)
        printf("\n");
    return (0);
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
        if (str_cmp(cmd, shell->builtins->name) == 0)
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
    if (str_cmp(cmd, "cd") == 0 || str_cmp(cmd, "echo") == 0)
        return (1);
    else if (str_cmp(cmd, "exit") == 0 || str_cmp(cmd, "pwd") == 0)
        return (1);
    else if (str_cmp(cmd, "export") == 0 || str_cmp(cmd, "unset") == 0)
        return (1);
    else if (str_cmp(cmd, "env") == 0)
        return (1);
    return (0);
}
