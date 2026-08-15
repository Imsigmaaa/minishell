/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 00:08:29 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/16 03:36:03 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	expand_heredoc_input(t_shell *shell, t_redir *redir, char **line)
{
	char	*expanded;

	if (!redir->heredoc_expand)
		return (1);
	expanded = expand_heredoc_line(*line, shell);
	free(*line);
	*line = expanded;
	if (!expanded)
		return (0);
	return (1);
}

static	int	write_heredoc(t_shell *shell, t_redir *redir, int fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_heredoc_warning(redir->target);
			break ;
		}
		if (env_strcmp(line, redir->target) == 0)
		{
			free(line);
			break ;
		}
		if (!expand_heredoc_input(shell, redir, &line))
			return (0);
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (1);
}

static void	heredoc_child(t_shell *shell, t_redir *redir, int fd)
{
	int	result;

	if (!init_heredoc_signals())
	{
		close(fd);
		exit(1);
	}
	result = write_heredoc(shell, redir, fd);
	close(fd);
	if (!result)
		exit(1);
	exit(0);
}

pid_t	run_heredoc_child(t_shell *shell, t_redir *redir, int fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		heredoc_child(shell, redir, fd);
	if (pid == -1)
	{
		perror("fork");
		shell->exit_status = 1;
		return (-1);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (pid);
}

int	wait_heredoc_child(pid_t pid, int *status)
{
	pid_t	result;

	result = waitpid(pid, status, 0);
	while (result == -1 && errno == EINTR)
		result = waitpid(pid, status, 0);
	if (result == -1)
	{
		perror("waitpid");
		return (0);
	}
	return (1);
}
