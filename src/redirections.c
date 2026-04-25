/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:25:13 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/25 13:27:52 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void apply_redirect_out(t_redirect *redir, t_shell *shell)
{
    int fd;

    fd = open(redir->target_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (redir_error(fd))
        return ;
    dup2(fd, STDOUT_FILENO); //standart çıktıya gidecekler artık fd ye gitsin
    close(fd); //fd yi kapatıyoruz çünkü artık standart çıktı yönlendirilmiş durumda
    //hedef dosyayı bir tane dosya açarak yönlendirmesini yaptıktan sonra artık kullanılması bitmiş oluyor.
}

void apply_redirect_in(t_redirect *redir, t_shell *shell)
{
    //burada okumayı yani stdin i hedef dosyayay yönelendircez
    int fd;

    fd = open(redir->target_file, O_RDONLY);
    if (redir_error(fd))
        return ;
    dup2(fd, STDIN_FILENO);
    close(fd); //hedef dosyayı yöneldirdik bitti kapat gari
}

void apply_heredoc(t_redirect *redir, t_shell *shell)
{
    //şimdii burada stdin (0) e yazdıklarım yeni bir dosyaya birikecek EOF kadar sonra o dosyadan stdout (1) a yönlendirilecek
    int fd;
    char *temp_file;
    char *line;

    fd = open(temp_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (redir_error(fd))
        return ;
    while (1)
    {
        line = readline("> ");
        if (line == NULL || str_cmp(line, redir->target_file) == 0)
            break;
        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    free(line);
    close(fd); //geçici dosyama write ile yazdırdım artık işim bitiyor tekrar açmalıyım
    //ama yanlızca okumak için terminalden okumasın da geçici dosyadan okusun diye dup2 ile stdin de geçici dosya da aynı yöne yönlendirildi
    fd = open(temp_file, O_RDONLY);
    if (redir_error(fd))
        return ;
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void apply_append(t_redirect *redir, t_shell *shell)
{
    int fd;

    fd = open(redir->target_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (redir_error(fd))
        return ;
    dup2(fd, STDOUT_FILENO); //artık hem stdout hem de fd aynı yere bakıyor yani hedef dosyaya yönlendirilmiş durumda yalnız bu sefer truncate yapmadık
    close(fd); //yani hedef dosyanın içeriği silinmediği için append modunda açtık
}

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
