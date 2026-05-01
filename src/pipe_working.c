#include "minishell.h"

int **create_pipes(int pipe_count)
{
    int **fd;
    int i;

    fd = malloc(sizeof(int *) * pipe_count);
    i = 0;
    while (i < pipe_count)
    {
        fd[i] = malloc(sizeof(int) * 2);
        if (pipe(fd[i]) == -1)
        {
            perror("pipe");
            exit(1);
        }
        i++;
    }
    return (fd);
}

void wait_for_children(t_shell *shell, int *how_died, pid_t *pid)
{
    int i;

    i = 0;
    while (i < shell->cmds->argc)
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
    t_pipe pipes;
    pid_t *pid;
    int how_died;
    int i;

    i = 0;
    how_died = 0;
    shell->pipes.fd = create_pipes(pipes.pipe_count);
    pid = malloc(sizeof(pid_t) * pipes.command_count); //child process sayısı kadar pid tutacak bir dizi
    while (pipes.pipe_count > i) //komut sayısı - 1 kadar pipe oluştur
    {
        pid[i] = fork();
        if (pid[i] == -1)
        {
            perror("fork");
            exit(1);
        }
        if (pid[i] == 0) //child process
        {
            connect_child_fds();
            apply_redir(shell->cmds[i].redirects, shell);
            update_env_nodes(shell);
            execute_command(shell->cmds[i], shell);
            exit(0);
        }
        i++;
    }
    close_pipes(pipes, pipes.pipe_count, 0); //parent process tüm pipe'ları kapatır çünkü artık kullanmayacak
    wait_for_children(shell, &how_died, pid); //parent process tüm child processlerin bitmesini bekler
}
