/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 23:26:44 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/13 23:27:13 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void close_created_fd(t_exec *exec, int created_pipes)
{
	int	j;
	j = 0;
	while (j < created_pipes)
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
