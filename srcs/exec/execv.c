/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execv.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 00:22:53 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/16 02:24:05 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	set_heredoc_error_status(t_shell *shell)
{
	close_all_heredoc_fds(shell);
	if (g_signal == SIGINT)
		shell->exit_status = 130;
	else
		shell->exit_status = 1;
	g_signal = 0;
}

int	executor(t_shell *shell)
{
	if (!shell->cmds)
		return (shell->exit_status);
	if (!prepare_all_heredocs(shell))
	{
		set_heredoc_error_status(shell);
		return (shell->exit_status);
	}
	if (count_cmds(shell->cmds) == 1)
		exec_single(shell);
	else
		exec_pipe(shell);
	close_all_heredoc_fds(shell);
	return (shell->exit_status);
}
