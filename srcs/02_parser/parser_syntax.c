/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 02:37:01 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/23 17:00:54 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <unistd.h>

int	syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	return (0);
}

static	int	check_pipe(t_token **toks)
{
	t_token	*next;

	next = (*toks)->next;
	if (!next || next->type == TOKEN_PIPE
		|| next->type == TOKEN_EOF)
		return (0);
	*toks = next;
	return (1);
}

static	int	check_redir(t_token **toks)
{
	t_token	*next;

	next = (*toks)->next;
	if (!next || next->type != TOKEN_WORD)
		return (0);
	*toks = next;
	return (1);
}

int	syntax_check(t_token *tokens)
{
	t_token	*toks;

	toks = tokens;
	if (!tokens)
		return (1);
	if (toks->type == TOKEN_PIPE)
		return (syntax_error("|"));
	while (toks && toks->type != TOKEN_EOF)
	{
		if (toks->type == TOKEN_WORD)
			toks = toks->next;
		else if (toks->type == TOKEN_PIPE)
		{
			if (check_pipe(&toks))
				return (syntax_error("|"));
		}
		else if (is_redir_token(toks->type))
		{
			if (check_redir(&toks))
				return (syntax_error(toks->value));
		}
		else
			return (syntax_error(toks->value));
	}
	return (1);
}
