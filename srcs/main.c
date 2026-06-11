/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:06:26 by yushan            #+#    #+#             */
/*   Updated: 2026/06/11 23:32:35 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void print_tokens(t_token *tokens)
{
	t_token *tmp;

	tmp = tokens;
	while (tmp)
	{
		printf("value       : %s\n",tmp->value);
		printf("type        : %d\n",tmp->type);
		printf("has_quote   : %d\n",tmp->has_quote);
		printf("has_wildcard: %d\n",tmp->has_wildcard);
		tmp = tmp->next;
	}
	
}
void print_cmds(t_cmd *cmds)
{
	t_cmd *tmp;

	tmp =  cmds;
	while (tmp)
	{
		int i = 0;
		while (tmp->argv &&tmp->argv[i])
		{
			printf("%s\n",tmp->argv[i]);
			i ++;
		}
		t_redir *redir = tmp->redirs;
		while (redir)
		{
			printf("%d\n",redir->type);
			printf("%s\n",redir->target);
			redir = redir->next;	
		}
		tmp = tmp->next;	
	}
}
int main(void)
{
	t_token *tokens;
	t_cmd	*cmds;	
	int err = 0;
	cmds = NULL;
	tokens = lexer("echo &&&", &err);
	if (err)
	{
		printf("lexer error\n");
		free_tokens(&tokens);
		return (1);
	}
	print_tokens(tokens);
	if(!syntax_check(tokens))
		return(free_tokens(&tokens),free_cmds(cmds), 0);
	cmds = parse_tokens(tokens);
	print_cmds(cmds);
	free_cmds(cmds);
	free_tokens(&tokens);

}
/*
void    process_line(t_shell *shell, char *line)
{
    int err;

	err = LEX_OK;
	shell->tokens = lexer(line, &err);
	if (err != LEX_OK)
	{
		print_lexer_error(err);
		if (err == LEX_ERR_MALLOC)
			shell->exit_status = 1;
		else
			shell->exit_status = 2;
		return ;
	}*/

	/*
		下一步：
		shell->ast = parser(shell->tokens);
		expansion(shell);
		execute(shell);
		cleanup current line data;
	*/
	//free_tokens(&shell->tokens);
//}
/*
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
*/