/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:35:00 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/16 00:37:14 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

static int	check_heredoc_status(t_shell *shell, int status)
{
	if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
		shell->exit_status = 1;
		return (0);
	}
	return (1);
}

static int	prepare_one_heredoc(t_shell *shell, t_redir *redir, int number)
{
	char	*path;
	int		write_fd;
	int		status;
	pid_t	pid;

	path = NULL;
	write_fd = open_heredoc_file(&path, number);
	if (write_fd == -1)
	{
		perror("heredoc");
		shell->exit_status = 1;
		return (0);
	}
	pid = run_heredoc_child(shell, redir, write_fd);
	close(write_fd);
	if (pid == -1)
		return (remove_heredoc_file(path), 0);
	if (!wait_heredoc_child(pid, &status))
		return (remove_heredoc_file(path), 0);
	if (!check_heredoc_status(shell, status))
		return (remove_heredoc_file(path), 0);
	return (open_heredoc_read(shell, redir, path));
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
			if (redir->type == TOKEN_HEREDOC)
			{
				if (!prepare_one_heredoc(shell, redir, number))
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
