/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 23:25:23 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/23 17:09:43 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_exec(t_exec *exec)
{
	exec->cmd_count = 0;
	exec->pipe_fd = NULL;
	exec->pids = NULL;
	exec->saved_stdin = - 1;
	exec->saved_stdout = -1;
	exec->status = 0;
}

int	init_exec_data(t_shell *shell, t_exec *exec)
{
	init_exec(exec);
	exec->cmd_count = count_cmds(shell->cmds);
	exec->pids = malloc(sizeof(pid_t) * exec->cmd_count);
	if (!exec->pids)
	{
		perror("malloc");
		shell->exit_status = 1;
		return (0);
	}
	if (exec->cmd_count > 1)
	{
		exec->pipe_fd = malloc(sizeof(int [2]) * (exec->cmd_count - 1));
		if (!exec->pipe_fd)
		{
			perror("malloc");
			free(exec->pids);//释放
			exec->pids = NULL;
			shell->exit_status = 1;
			return (0);
		}
	}
	return (1);
}

int	create_pipes(t_shell *shell, t_exec *exec)
{
	int		i;

	i = 0;
	while (i < exec->cmd_count - 1)
	{
		if (pipe(exec->pipe_fd[i]) == -1)
		{
			close_created_fd(exec, i);//释放所有已创建pipe
			perror("pipe");
			shell->exit_status = 1;
			return (0);
		}
		i ++;
	}
	return (1);
}
