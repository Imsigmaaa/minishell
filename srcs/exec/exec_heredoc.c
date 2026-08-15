/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:35:00 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/15 23:41:04 by xingchen         ###   ########.fr       */
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
	path = ft_strjoin("/tmp/heredoc_tmp_", num);
	free(num);
	return (path);
}
static	int	open_heredoc_file(char **path, int number)
{
	int fd;
	int saved_errno;
	//难点 errno 具体了解
	while (1)
	{
		*path = get_heredoc_path(number);
		if (!*path)
			return (errno = ENOMEM, -1);
		fd = open(*path, O_WRONLY | O_CREAT |  O_EXCL, 0600);
	if (fd >= 0)
		return (fd);
	saved_errno = errno;
	free(*path);
	*path = NULL;
	if (saved_errno != EEXIST)
		return (errno = saved_errno, -1);
	number ++;
	}
}
static	void	print_heredoc_warning(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
}

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

static	int	write_heredoc(t_shell *shell,  t_redir *redir, int fd)
{
	char	*line;
	
	while (1)
	{
		line = readline("> ");//函数内部给malloc了
		if (!line)
		{
			print_heredoc_warning(redir->target);
			break ;
		}
		if (ft_strcmp(line, redir->target) == 0)
		{
			free(line);
			break;
		}
		if(!expand_heredoc_input(shell, redir, &line))
			return (close(fd), 0);
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	return (1);
}

static void	heredoc_child(t_shell *shell, t_redir *redir, int fd,
	char *path)
{
	int	result;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	result = write_heredoc(shell, redir, fd);
	free(path);
	exit(!result);
}

static	int	wait_heredoc_child(pid_t pid, int *status)
{
	pid_t result;

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
static	void	remove_heredoc_file(char *path)
{
	if (!path)
		return ;
	unlink(path);
	free(path);
}

static int	run_heredoc_child(t_shell *shell, t_redir *redir, int fd,
	char *path, int *status)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		heredoc_child(shell, redir, fd, path);
	close(fd);
	if (pid == -1)
	{
		perror("fork");
		shell->exit_status = 1;
		init_interactive_signals();
		return (0);
	}
	if (!wait_heredoc_child(pid, status))
	{
		shell->exit_status = 1;
		init_interactive_signals();
		return (0);
	}
	init_interactive_signals();
	return (1);
}
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

static int	open_heredoc_read(t_shell *shell, t_redir *redir, char *path)
{
	redir->heredoc_fd = open(path, O_RDONLY);
	remove_heredoc_file(path);
	if (redir->heredoc_fd == -1)
	{
		perror("heredoc");
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

	path = NULL;
	write_fd = open_heredoc_file(&path, number);
	if (write_fd == -1)
	{
		perror("heredoc");
		shell->exit_status = 1;
		return (0);
	}
	if (!run_heredoc_child(shell, redir, write_fd, path, &status))
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
