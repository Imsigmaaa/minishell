/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 02:31:49 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/19 09:06:37 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	wait_one_child(t_exec *exec, int i)
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

void	wait_all_children(t_shell *shell, t_exec *exec, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (!(wait_one_child(exec, i)))
		{
			if (i == count - 1)
				shell->exit_status = 1;
		}
		else if (i == count - 1)
		{
			print_child_signal(exec->status);
			update_exit_status(shell, exec->status);
		}
		i ++;
	}
}
