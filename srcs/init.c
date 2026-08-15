/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:28:17 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 21:37:30 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int	init_shell(t_shell *shell, char **envp)
{
	shell->env = NULL;
	shell->tokens = NULL;
	shell->cmds = NULL;
	shell->exit_status = 0;
	shell->should_exit = 0;
	shell->env = env_init(envp);
	if (envp && envp[0] && !shell->env)
	{
		perror("malloc");
		return (0);
	}
	return (1);
}

void	free_line_data(t_shell *shell)
{
	free_tokens(&shell->tokens);
	free_cmds(shell->cmds);
	shell->tokens = NULL;
	shell->cmds = NULL;
}

void	cleanup_shell(t_shell *shell)
{
	free_line_data(shell);
	env_free(&shell->env);
	rl_clear_history();
}
