/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:35:00 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/15 22:04:03 by xingchen         ###   ########.fr       */
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

static	int	write_heredoc(t_shell *shell,  t_redir *redir, int fd)
{
	char	*line;
	char	*expanded;
	
	g_signal = 0;
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");//函数内部给malloc了
		if (g_signal == SIGINT)
		{
			free(line);
			close(fd);
			shell->exit_status = 130;
			init_interactive_signals();
			return (0);
		}
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document ", 2);
			ft_putstr_fd("delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(redir->target, 2);
			ft_putendl_fd("')", 2);
			break ;
		}
		if (ft_strcmp(line, redir->target) == 0)
		{
			free(line);
			break;
		}
		if (redir->heredoc_expand)
		{
			expanded = expand_heredoc_line(line, shell);
			free(line);
			if (!expanded)
			{
				close(fd);
				shell->exit_status = 1;
				init_interactive_signals();
				return (0);
			}
			line = expanded;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	init_interactive_signals();
	return (1);
}

static	int	prepare_one_heredoc(t_shell *shell, t_redir *redir, int number)
{
	char	*path;
	int		write_fd;
	
	path = get_heredoc_path(number);
	if (!path)
		return (0);
	write_fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (write_fd == -1)
		return (free(path), perror("heredoc"), 0);
	if (!write_heredoc(shell,redir, write_fd))
	{
		unlink(path);
		free(path);
		return (0);
	}
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
				if(!prepare_one_heredoc(shell, redir, number))
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
