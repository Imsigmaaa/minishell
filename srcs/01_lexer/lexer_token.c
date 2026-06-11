/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 22:29:33 by yushan            #+#    #+#             */
/*   Updated: 2026/06/10 00:19:53 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*new_token(t_token_type type, char *value,
	int has_quote, int has_wildcard)
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
	token->has_wildcard = has_wildcard;
	token->next = NULL;
	return (token);
}

int	add_token(t_lexer *lex, t_token_type type, char *value,
	int has_quote, int has_wildcard)
{
	t_token	*token;

	token = new_token(type, value, has_quote, has_wildcard);
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
