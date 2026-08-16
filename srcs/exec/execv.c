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

int	executor(t_shell *shell)
{
	if (!shell->cmds)
		return (shell->exit_status);
	if (!prepare_all_heredocs(shell))
	{
		close_all_heredoc_fds(shell);
		init_interactive_signals();
		return (shell->exit_status);
	}
	init_interactive_signals();
	if (count_cmds(shell->cmds) == 1)
		exec_single(shell);
	else
		exec_pipe(shell);
	close_all_heredoc_fds(shell);
	return (shell->exit_status);
}
