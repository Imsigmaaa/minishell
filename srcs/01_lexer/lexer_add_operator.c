/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_add_operator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 23:51:11 by yushan            #+#    #+#             */
/*   Updated: 2026/06/11 22:20:33 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int add_operator_token(t_lexer *lex, char *s, int *i)
{
	if (s[*i] == '|' && s[*i + 1] == '|')
		return (*i += 2, add_token(lex, TOKEN_OR, ms_strdup("||"), 0, 0));
	if (s[*i] == '|')
		return (*i += 1, add_token(lex, TOKEN_PIPE, ms_strdup("|"), 0, 0));
	if (s[*i] == '&' && s[*i + 1] == '&')
		return (*i += 2, add_token(lex, TOKEN_AND, ms_strdup("&&"), 0, 0));
	if (s[*i] == '&' || s[*i] == ';')
	{
		lex->err = LEX_ERR_UNSUPPORTED_TOKEN;
		return (0);
	}
	if (s[*i] == '<' && s[*i + 1] == '<')
		return (*i += 2, add_token(lex, TOKEN_HEREDOC, ms_strdup("<<"), 0, 0));
	if (s[*i] == '<')
		return (*i += 1, add_token(lex, TOKEN_REDIR_IN, ms_strdup("<"), 0, 0));
	if (s[*i] == '>' && s[*i + 1] == '>')
		return (*i += 2, add_token(lex, TOKEN_APPEND, ms_strdup(">>"), 0, 0));
	if (s[*i] == '>')
		return (*i += 1, add_token(lex, TOKEN_REDIR_OUT, ms_strdup(">"), 0, 0));
	if (s[*i] == '(')
		return (*i += 1, add_token(lex, TOKEN_LPAREN, ms_strdup("("), 0, 0));
	return (*i += 1, add_token(lex, TOKEN_RPAREN, ms_strdup(")"), 0, 0));
}
