/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execv.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 00:22:53 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/15 22:17:05 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	executor(t_shell *shell)
{
	int	n_cmds;

	if (!shell->cmds)
		return (shell->exit_status);
	if (!prepare_all_heredocs(shell))
	{
		close_all_heredoc_fds(shell);
		if (g_signal == SIGINT)
			shell->exit_status = 130;
		else if (shell->exit_status != 130)
			shell->exit_status = 1;
		g_signal = 0;
		return (shell->exit_status);
	}
	n_cmds = count_cmds(shell->cmds);
	if (n_cmds == 1)
		exec_single(shell);
	else
		exec_pipe(shell);
	close_all_heredoc_fds(shell);
	return (shell->exit_status);
	
}
