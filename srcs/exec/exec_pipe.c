/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:17:37 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/16 02:23:35 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dup_pipe_fd(int i, t_exec *exec)
{
	if (i == 0)
	{
		if (dup2(exec->pipe_fd[0][1], STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
	}
	else if (i == exec->cmd_count - 1)
	{
		if (dup2(exec->pipe_fd[i - 1][0], STDIN_FILENO) == -1)
			return (perror("dup2"), 0);
	}
	else
	{
		if (dup2(exec->pipe_fd[i - 1][0], STDIN_FILENO) == -1
		|| dup2(exec->pipe_fd[i][1], STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
	}
	return (1);
}

void	close_free_and_exit_child(t_shell *shell, t_exec *exec)
{
	close_created_fd(exec, exec->cmd_count - 1);
	close_all_heredoc_fds(shell);
	free_exec(exec);
	exit(1);
}

static void	exec_child(t_shell *shell, t_cmd *cmd, int i, t_exec *exec)
{
	int	status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!dup_pipe_fd(i, exec))
		close_free_and_exit_child(shell, exec);
	if (cmd->redirs && exec_redir(cmd) == -1)
		close_free_and_exit_child(shell, exec);
	close_created_fd(exec, exec->cmd_count - 1);
	close_all_heredoc_fds(shell);
	if (!cmd->argv || !cmd->argv[0])
	{
		free_exec(exec);
		exit(0);
	}
	if (is_builtin(cmd))
	{
		status = exec_builtin(shell, cmd);
		free_exec(exec);
		exit(status);
	}
	free_exec(exec);
	exec_cmd(cmd, shell->env);
	exit(126);
}

int	wait_one_child(t_exec *exec, int i)
{
	int	wait_result;

	wait_result = waitpid(exec->pids[i], &exec->status, 0);
	while (wait_result == -1 && errno == EINTR)
		wait_result = waitpid(exec->pids[i], &exec->status, 0);
	if (wait_result == -1)
	{
		perror("waitpid");
		return (0);
	}
	return (1);
}

static	void	wait_all_childs(t_shell *shell, t_exec *exec, int count)
{
	int	wait_error;
	int	i;

	wait_error = 0;
	i = 0;
	while (i < count)
	{
		if (!(wait_one_child(exec, i)))
			wait_error = 1;
		else if (i == count - 1)
		{
			print_child_signal(exec->status);
			update_exit_status(shell, exec->status);
		}
		i ++;
	}
	if (wait_error)
		shell->exit_status = 1;
	return ;
}

static	int	fork_children(t_shell *shell, t_exec *exec)
{
	int		i;
	t_cmd	*cur;

	i = 0;
	cur = shell->cmds;
	while (i < exec->cmd_count)
	{
		exec->pids[i] = fork();
		if (exec->pids[i] == -1)
		{
			perror("fork");
			close_created_fd(exec, exec->cmd_count - 1);
			wait_all_childs(shell, exec, i);
			shell->exit_status = 1;
			return (0);
		}
		if (exec->pids[i] == 0)
			exec_child(shell, cur, i, exec);
		cur = cur->next;
		i ++;
	}
	return (1);
}

void	exec_pipe(t_shell *shell)
{
	t_exec	exec;

	if (!init_exec_data(shell, &exec) || !create_pipes(shell, &exec))
	{
		free_exec(&exec);
		return ;
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (!fork_children(shell, &exec))
	{
		init_interactive_signals();
		free_exec(&exec);
		return ;
	}
	close_created_fd(&exec, exec.cmd_count - 1);
	wait_all_childs(shell, &exec, exec.cmd_count);
	init_interactive_signals();
	free_exec(&exec);
}
