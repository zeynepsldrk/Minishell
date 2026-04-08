#include "minishell.h"

void apply_redirect_out(redir, shell)
{
    int fd;

    fd = open(redir->target_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("open");
        return ;
    }
    dup2(fd, STDOUT_FILENO); //standart çıktıya gidecekler artık fd ye gitsin
    close(fd); //fd yi kapatıyoruz çünkü artık standart çıktı yönlendirilmiş durumda
}

void apply_redirect_in(redir, shell)
{
    
}

void apply_heredoc(redir, shell)
{

}

void apply_append(redir, shell)
{

}
