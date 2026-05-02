#include "minishell.h"

/*yapılacaklar
-sinyallerin nasıl işlediğini araştır
*/

int **create_pipes(int pipe_count)
{
    int **fd;
    int i;

    fd = malloc(sizeof(int *) * pipe_count);
    if (!fd)
        return (NULL);
    i = 0;
    while (i < pipe_count)
    {
        fd[i] = malloc(sizeof(int) * 2);
        if (!fd[i] || pipe(fd[i]) == -1)
        {
            perror("pipe");
            exit(1);
        }
        i++;
    }
    return (fd);
}

void execute_command(t_shell *shell)
{
    if (is_builtin(shell->cmds->argv[0], shell->list_builtin))
        execute_builtin(shell->cmds, shell, 0);
    else
        execute_external(shell);
}

void connect_child_fds(int i, int cmd_count, int **fd)
{
    if (i == 0) //ilk komut
        dup2(fd[i][1], STDOUT_FILENO); //stdout'u pipe'ın yazma ucuna yönlendir
    else if (i == cmd_count - 1) //son komut
        dup2(fd[i - 1][0], STDIN_FILENO); //stdin'i önceki pipe'ın okuma ucuna yönlendir
    else //ortadaki komutlar
    {
        dup2(fd[i - 1][0], STDIN_FILENO); //stdin'i önceki pipe'ın okuma ucuna yönlendir
        dup2(fd[i][1], STDOUT_FILENO); //stdout'u pipe'ın yazma ucuna yönlendir
    }
    close_all_pipes(fd, cmd_count - 1, 0); //child process tüm pipe'ları kapatır çünkü artık yönlendirme yapılmıştır
}

void wait_for_children(t_shell *shell, int *how_died, pid_t *pid)
{
    int i;

    i = 0;
    while (i < shell->pipes.command_count)
    {
        waitpid(pid[i], how_died, 0);
        i++;
    }
    free(pid);
}

void close_all_pipes(int **fd, int pipe_count, int i)
{
    while (i < pipe_count)
    {
        close(fd[i][0]);
        close(fd[i][1]);
        i++;
    }
}

void pipe_working(t_shell *shell)
{
    pid_t *pid;
    int how_died;
    int i;
    t_cmd *cmd;

    i = 0;
    how_died = 0;
    cmd = shell->cmds;
    shell->pipes.fd = create_pipes(shell->pipes.pipe_count);
    pid = malloc(sizeof(pid_t) * shell->pipes.command_count); //child process sayısı kadar pid tutacak bir dizi
    while (shell->pipes.command_count > i) //komut sayısı - 1 kadar pipe oluştur
    {
        pid[i] = fork();
        if (pid[i] == -1)
        {
            perror("fork");
            exit(1);
        }
        if (pid[i] == 0) //child process
        {
            shell->cmds = cmd;
            connect_child_fds(i, shell->pipes.command_count, shell->pipes.fd);
            if (apply_redir(shell->cmds->redirects, shell))
                exit(1);
            update_env_nodes(shell);
            execute_command(shell);
            exit(0);
        }
        cmd = cmd->next; //her fork da parent bir sonraki node a geçer child o anki cmd ile çalışır
        i++;
    }
    close_all_pipes(shell->pipes.fd, shell->pipes.pipe_count, 0); //parent process tüm pipe'ları kapatır çünkü artık kullanmayacak
    wait_for_children(shell, &how_died, pid); //parent process tüm child processlerin bitmesini bekler
}
