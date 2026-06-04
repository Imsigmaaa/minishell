/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:06:26 by yushan            #+#    #+#             */
/*   Updated: 2026/06/04 14:40:10 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

void    process_line(t_shell *shell, char *line)
{
    tokenize(line);
    parse_tokens();
    expand_commands();
    execute_commands(shell);
    cleanup_current_command();
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    char    *line;

    init_shell(&shell, envp);
    setup_interactive_signals();
    while (1)
    {
        line = readline("minishell$ ");
        if (line == NULL)
            break;
        if (*line != '\0')
            add_history(line);
        process_line(&shell, line);
        free(line);
    }
    cleanup_shell(&shell);
    return (shell.exit_status);
}
