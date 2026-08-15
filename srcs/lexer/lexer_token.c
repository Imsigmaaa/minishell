/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 22:29:33 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 12:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*new_token(t_token_type type, char *value, int has_quote)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(value);
		return (NULL);
	}
	token->value = value;
	token->type = type;
	token->has_quote = has_quote;
	token->next = NULL;
	return (token);
}

/* MODIFIED: reject failed token-value allocations before parsing. */
int	add_token(t_lexer *lex, t_token_type type, char *value, int has_quote)
{
	t_token	*token;

	if (type != TOKEN_EOF && !value)
	{
		lex->err = LEX_ERR_MALLOC;
		return (0);
	}
	token = new_token(type, value, has_quote);
	if (!token)
	{
		lex->err = LEX_ERR_MALLOC;
		return (0);
	}
	if (!lex->head)
		lex->head = token;
	else
		lex->tail->next = token;
	lex->tail = token;
	return (1);
}
