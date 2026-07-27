/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:35:00 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/27 21:20:30 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char	*get_heredoc_path(int number)
{
	char	*num;
	char	*path;

	num = ft_itoa(number);
	if (!num)
		return (NULL);
	path = ft_strjoin("/tmp/heredoc_tmp_",num);
	free(num);
	return (path);
}

static	void	write_heredoc(t_redir *redir, int fd)
{
	char	*line;
	
	line = NULL;
	while (1)
	{
		line = readline("> ");//函数内部给malloc了
		if (!line || ft_strcmp(line, redir->target) == 0)
		{
			free(line);
			break;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	return ;
}

static	int	prepare_one_heredoc(t_redir *redir, int number)
{
	char	*path;
	int		write_fd;
	
	path = get_heredoc_path(number);
	if (!path)
		return (0);
	write_fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (write_fd == -1)
		return (free(path), perror("heredoc"), 0);
	write_heredoc(redir, write_fd);
	redir->heredoc_fd = open(path, O_RDONLY);
	unlink(path);
	free(path);
	if (redir->heredoc_fd == -1)
		return (perror("heredoc"), 0);
	return (1);
}

int	prepare_all_heredocs(t_shell *shell)
{
	t_cmd	*cmds;
	t_redir	*redir;
	int		number;

	cmds = shell->cmds;
	number = 0;
	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type ==TOKEN_HEREDOC)
			{
				if(!prepare_one_heredoc(redir, number))
					return (0);
				number ++;
			}
			redir = redir->next;
		}
		cmds = cmds->next;
	}
	return (1);
}

void	close_all_heredoc_fds(t_shell *shell)
{
	t_cmd	*cmds;
	t_redir	*redir;

	cmds = shell->cmds;
	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->heredoc_fd >= 0)
			{
				close(redir->heredoc_fd);
				redir->heredoc_fd = -1;
			}
			redir = redir->next;
		}
		cmds = cmds->next;
	}
}
