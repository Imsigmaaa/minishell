/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execv.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 00:22:53 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/23 17:22:18 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	executor(t_shell *shell)
{
	int	n_cmds;

	if (!shell->cmds)
		return (shell->exit_status);
	n_cmds = count_cmds(shell->cmds);
	if (n_cmds == 1)
		exec_single(shell);
	else
		exec_pipe(shell);
	return (shell->exit_status);
	
}
