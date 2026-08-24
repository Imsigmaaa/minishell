/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:12:35 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/23 18:08:55 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	saved_stdio(int saved[2])
{
	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	if (saved[0] == -1 || saved[1] == -1)
	{
		perror("dup");
		if (saved[0] >= 0)
			close(saved[0]);
		if (saved[1] >= 0)
			close(saved[1]);
		return (0);
	}
	return (1);
}

static	void	exec_in_parent(t_shell *shell)
{
	t_cmd	*cmd;
	int		saved[2];

	cmd = shell->cmds;
	if (!saved_stdio(saved))
	{
		shell->exit_status = 1;
		return ;
	}
	if (cmd->redirs && exec_redir(cmd) == -1)
		shell->exit_status = 1;
	else if (!cmd->argv || !cmd->argv[0])
		shell->exit_status = 0;
	else
		shell->exit_status = exec_builtin(shell, cmd);
	dup2(saved[0], STDIN_FILENO);
	dup2(saved[1], STDOUT_FILENO);
	close(saved[0]);
	close(saved[1]);
}

static void	exec_single_child(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = shell->cmds;
	default_signals();
	if (cmd->redirs && exec_redir(cmd) == -1)
	{
		close_all_heredoc_fds(shell);
		child_exit(shell, 1);
	}
	close_all_heredoc_fds(shell);
	child_exit(shell, exec_cmd(cmd, shell->env));
}

static void	wait_single_child(t_shell *shell, pid_t pid)
{
	int	status;

	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno != EINTR)
		{
			perror("waitpid");
			shell->exit_status = 1;
			return ;
		}
	}
	print_child_signal(status);
	update_exit_status(shell, status);
}

void	exec_single(t_shell *shell)
{
	pid_t	pid;

	if (!shell->cmds->argv || !shell->cmds->argv[0] || is_builtin(shell->cmds))
	{
		exec_in_parent(shell);
		return ;
	}
	ignore_signals();
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		shell->exit_status = 1;
	}
	else if (pid == 0)
		exec_single_child(shell);
	else
	{
		wait_single_child(shell, pid);
		close_all_heredoc_fds(shell);
	}
	init_interactive_signals();
}
