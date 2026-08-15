/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 12:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 13:07:58 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	lex_line(t_shell *shell, char *line)
{
	int	err;

	err = LEX_OK;
	shell->tokens = lexer(line, &err);
	if (err != LEX_OK)
	{
		print_lexer_error(err);
		if (err == LEX_ERR_MALLOC)
			shell->exit_status = 1;
		else
			shell->exit_status = 2;
		return (0);
	}
	if (!syntax_check(shell->tokens))
	{
		shell->exit_status = 2;
		return (0);
	}
	return (1);
}

static int	parse_line(t_shell *shell, char *line)
{
	if (!lex_line(shell, line))
		return (0);
	shell->cmds = parse_tokens(shell->tokens);
	if (!shell->cmds && shell->tokens->type != TOKEN_EOF)
	{
		shell->exit_status = 1;
		return (0);
	}
	return (shell->cmds != NULL);
}

/* MODIFIED: expansion must succeed before execution begins. */
static void	process_line(t_shell *shell, char *line)
{
	if (parse_line(shell, line) && expansion(shell))
		executor(shell);
	free_cmds(shell->cmds);
	shell->cmds = NULL;
	free_tokens(&shell->tokens);
}

static int	read_input(t_shell *shell)
{
	char	*line;

	line = readline("minishell$ ");
	sync_signal_status(shell);
	if (!line)
	{
		write(1, "exit\n", 5);
		return (0);
	}
	if (line[0])
		add_history(line);
	process_line(shell, line);
	free(line);
	return (1);
}

/* MODIFIED: prompt signal behavior is installed before the readline loop. */
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	if (argc != 1)
		return (write(2, "Usage: ./minishell\n", 19), 1);
	if (!init_shell(&shell, envp))
		return (1);
	if (!init_interactive_signals())
		return (cleanup_shell(&shell), 1);
	while (!shell.should_exit && read_input(&shell))
		;
	cleanup_shell(&shell);
	return (shell.exit_status);
}
