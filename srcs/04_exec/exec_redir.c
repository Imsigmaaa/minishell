/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:17:54 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/27 21:47:00 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_redir_fd(t_redir *redir, int fd)
{
	close(fd);
	if (redir->type == TOKEN_HEREDOC)
		redir->heredoc_fd = -1;
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
		fd = redir->heredoc_fd;
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
			return(perror("dup2"),close_redir_fd(redir, fd), -1);
		close_redir_fd(redir, fd);
		redir = redir->next;
	}
	return (0);
}