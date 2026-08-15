/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_add_operator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 23:51:11 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 12:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unsupported_operator(t_lexer *lex, int *index, int size)
{
	*index += size;
	lex->err = LEX_ERR_UNSUPPORTED_TOKEN;
	return (0);
}

int	add_operator_token(t_lexer *lex, char *input, int *index)
{
	if (input[*index] == '|' && input[*index + 1] == '|')
		return (unsupported_operator(lex, index, 2));
	if (input[*index] == '|')
		return (*index += 1, add_token(lex, TOKEN_PIPE,
				ms_strdup("|"), 0));
	if (input[*index] == '&' || input[*index] == ';'
		|| input[*index] == '(' || input[*index] == ')')
		return (unsupported_operator(lex, index, 1));
	if (input[*index] == '<' && input[*index + 1] == '<')
		return (*index += 2, add_token(lex, TOKEN_HEREDOC,
				ms_strdup("<<"), 0));
	if (input[*index] == '<')
		return (*index += 1, add_token(lex, TOKEN_REDIR_IN,
				ms_strdup("<"), 0));
	if (input[*index] == '>' && input[*index + 1] == '>')
		return (*index += 2, add_token(lex, TOKEN_APPEND,
				ms_strdup(">>"), 0));
	return (*index += 1, add_token(lex, TOKEN_REDIR_OUT,
			ms_strdup(">"), 0));
}
