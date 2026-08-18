/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execv.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 00:22:53 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/19 03:12:12 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	executor(t_shell *shell)
{
	if (!shell->cmds)
		return ;
	if (!prepare_all_heredocs(shell))
	{
		close_all_heredoc_fds(shell);
		init_interactive_signals();
			return ;
	}
	init_interactive_signals();
	if (count_cmds(shell->cmds) == 1)
		exec_single(shell);
	else
		exec_pipe(shell);
	close_all_heredoc_fds(shell);
}
