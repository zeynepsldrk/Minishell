/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_working.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 14:11:39 by zedurak           #+#    #+#             */
/*   Updated: 2026/05/02 22:09:07 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	wait_for_children(t_shell *shell, int *how_died, pid_t *pid)
{
	int	i;

	i = 0;
	while (i < shell->pipes.command_count)
	{
		waitpid(pid[i], how_died, 0);
		i++;
	}
	/*how_died değerini direkt çıkış durumu olarak kullanamıyoruz çünkü waitpid içinde çıkış kodu dışında
	farklı değerler de tutuyor. MAkro kullanarak doğru değerleri görmeyi sağlar.*/
	if (WIFEXITED(*how_died)) //Normal mi çıktı 1 veya 0
		shell->exit_status = WEXITSTATUS(*how_died); //normalse kaçla çıkış yaptı
	else if (WIFSIGNALED(*how_died)) //Sinyalle mi öldü 1 veya 0
		shell->exit_status = 128 + WTERMSIG(*how_died); //sinyal kaçla çıktı
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

    how_died = 0;
    shell->pipes.fd = create_pipes(shell->pipes.pipe_count);
    pid = malloc(sizeof(pid_t) * shell->pipes.command_count); //child process sayısı kadar pid tutacak bir dizi
    spawn_commands(shell, pid, 0);
    close_all_pipes(shell->pipes.fd, shell->pipes.pipe_count, 0); //parent process tüm pipe'ları kapatır çünkü artık kullanmayacak
    wait_for_children(shell, &how_died, pid); //parent process tüm child processlerin bitmesini bekler
}
