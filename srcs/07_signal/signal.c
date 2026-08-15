/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 15:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 15:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal;

static void	handle_prompt_sigint(int signal_number)
{
	g_signal = signal_number;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	init_interactive_signals(void)
{
	struct sigaction	action;

	g_signal = 0;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	action.sa_handler = handle_prompt_sigint;
	if (sigaction(SIGINT, &action, NULL) < 0)
		return (perror("sigaction"), 0);
	action.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &action, NULL) < 0)
		return (perror("sigaction"), 0);
	return (1);
}

void	sync_signal_status(t_shell *shell)
{
	if (g_signal == SIGINT)
		shell->exit_status = 130;
	g_signal = 0;
}
