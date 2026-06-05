/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:25:13 by zedurak           #+#    #+#             */
/*   Updated: 2026/05/16 19:02:09 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int apply_redirect_out(t_redirect *redir)
{
	int fd;

	fd = open(redir->target_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (ft_redir_error(fd))
		return (1);
	if (ft_safe_dup2(fd, STDOUT_FILENO) == -1) //stadart çıktıya değil fd ye gitsin
        return (1);
	close(fd); //fd yi kapatıyoruz çünkü artık standart çıktı yönlendirilmiş durumda
	//hedef dosyayı bir tane dosya açarak yönlendirmesini yaptıktan sonra artık kullanılması bitmiş oluyor.
	return (0);
}

int apply_redirect_in(t_redirect *redir)
{
	//burada okumayı yani stdin i hedef dosyayay yönelendircez
	int fd;

	fd = open(redir->target_file, O_RDONLY);
	if (ft_redir_error(fd))
		return (1);
	if (ft_safe_dup2(fd, STDIN_FILENO) == -1)
		return (1);
	close(fd); //hedef dosyayı yöneldirdik bitti kapat gari
	return (0);
}

int apply_heredoc(t_redirect *redir)
{
	//şimdii burada stdin (0) e yazdıklarım yeni bir dosyaya birikecek EOF kadar sonra o dosyadan stdout (1) a yönlendirilecek
	char *line;

	if (pipe(redir->heredoc_fd) == -1)
		return (1);
	while (1)
	{
		if (g_signal == SIGINT)
			break;
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, redir->target_file) == 0)
		{
			free(line);
			break;
		}
		write(redir->heredoc_fd[1], line, ft_strlen(line));
		write(redir->heredoc_fd[1], "\n", 1);
		free(line);
	}
	close(redir->heredoc_fd[1]);
	if (ft_safe_dup2(redir->heredoc_fd[0], STDIN_FILENO) == -1)
		return (1);
	close(redir->heredoc_fd[0]);
	return (0);
	//pipe kullandım fd nin 1 i yazma ucudur. fd nin 0 ı standart girdiyi buraya bağladım ki okumayı pipe dan yapsın
}

int apply_append(t_redirect *redir)
{
	int fd;

	fd = open(redir->target_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (ft_redir_error(fd))
		return (1);
	if (ft_safe_dup2(fd, STDOUT_FILENO) == -1) //artık hem stdout hem de fd aynı yere bakıyor yani hedef dosyaya yönlendirilmiş durumda yalnız bu sefer truncate yapmadık
		return (1);
	close(fd); //yani hedef dosyanın içeriği silinmediği için append modunda açtık
	return (0);
}

int apply_redir(t_redirect *redir)
{
	while (redir)
	{
		if(redir->type == REDIRECT_IN)
		{
			if (apply_redirect_in(redir))
				return (1);
		}
		else if(redir->type == REDIRECT_OUT)
		{
			if (apply_redirect_out(redir))
				return (1);
		}
		else if(redir->type == HEREDOC)
		{
			if (apply_heredoc(redir))
				return (1);
		}
		else if(redir->type == APPEND)
		{
			if (apply_append(redir))
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}
