/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:17:54 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/23 17:20:36 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc(t_redir *redir)
{
	char	*line;
	int		hd_fd[2];
	
	if (pipe(hd_fd) == -1)
		return(-1);
	while (1)
	{
		line = readline("> ");//函数内部给malloc了
		if (!line || ft_strcmp(line, redir->target) == 0)
		{
			free(line);
			break;
		}
		write(hd_fd[1], line, ft_strlen(line));
		write(hd_fd[1], "\n", 1);
		free(line);
	}
	close(hd_fd[1]);
	return(hd_fd[0]);
}

int	prepare_redir_fd(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == TOKEN_REDIR_IN)
		fd = open(redir->target, O_RDONLY);
	else if (redir->type == TOKEN_REDIR_OUT)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_APPEND)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == TOKEN_HEREDOC)
		fd = handle_heredoc(redir);
	return (fd);
}

int	exec_redir(t_cmd *cmd)
{
	int		fd;
	int		c;
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_IN || redir->type == TOKEN_HEREDOC)
			c = STDIN_FILENO;
		else
			c = STDOUT_FILENO;
		fd = prepare_redir_fd(redir);
		if (fd == -1)
			return(perror(redir->target), -1);
		if (dup2(fd, c) == -1)
			return(perror("dup2"), close(fd), -1);
		close(fd);
		redir = redir->next;
	}
	return (0);
}