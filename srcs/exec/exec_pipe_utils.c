/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 23:26:44 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/24 20:01:17 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_created_fd(t_exec *exec, int pipe_count)
{
	int	j;

	j = 0;
	while (j < pipe_count)
	{
		close(exec->pipe_fd[j][0]);
		close(exec->pipe_fd[j][1]);
		j ++;
	}
}

void	free_exec(t_exec *exec)
{
	if (exec->pids)
		free(exec->pids);
	if (exec->pipe_fd)
		free(exec->pipe_fd);
	exec->pids = NULL;
	exec->pipe_fd = NULL;
	return ;
}

void	update_exit_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

int	count_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		count;

	tmp = cmds;
	count = 0;
	while (tmp)
	{
		tmp = tmp->next;
		count ++;
	}
	return (count);
}
