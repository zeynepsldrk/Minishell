/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 20:12:21 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/25 20:19:16 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *execute_builtin(char *cmd, t_shell *shell, int i) //cmd hem tek node hem de builtin ise zaten buraya gelmişizdir.
{                                          //Burada redir olup olmaması fark etmeksizin
    int backup_stdout;
    int backup_stdin;

    backup_stdout = dup(STDOUT_FILENO); //standart girdi/çıktıları önceden kaydediyoruz çünkü redirlere göre değerleri kaybolacak
    backup_stdin = dup(STDIN_FILENO);
    if(shell->cmds->redirects != NULL) //redir uygula -varsa tabii-
        apply_redir(shell->cmds->redirects, shell);
    while (shell->list_builtin[i].name) //komutu çalıştır
    {
        if (ft_strcmp(cmd, shell->list_builtin[i].name) == 0)
        {
            shell->list_builtin[i].func(shell);
            break ;
        }
        i++;
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
    if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0)
        return (1);
    else if (ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "pwd") == 0)
        return (1);
    else if (ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0)
        return (1);
    else if (ft_strcmp(cmd, "env") == 0)
        return (1);
    return (0);
}

char *my_little_getenv(t_env_node *env_list, char *key)
{
    t_env_node *temp;

    temp = env_list;
    while (temp)
    {
        if (ft_strcmp(temp->key, key) == 0)
            return (temp->value); // Değeri bulduk, adresi döndür
        temp = temp->next;
    }
    return (NULL); // Bulamazsak NULL dön
}
