/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_add_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 23:51:30 by yushan            #+#    #+#             */
/*   Updated: 2026/06/10 00:29:15 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_quote(char c, char *quote, int *has_quote)
{
	if (c == '\'' && *quote == 0)
	{
		*quote = '\'';
		*has_quote = 1;
	}
	else if (c == '\'' && *quote == '\'')
		*quote = 0;
	else if (c == '"' && *quote == 0)
	{
		*quote = '"';
		*has_quote = 1;
	}
	else if (c == '"' && *quote == '"')
		*quote = 0;
}

static int	scan_word_end(char *s, int start, int *has_quote,
	int *has_wildcard)
{
	int		i;
	char	quote;

	i = start;
	quote = 0;
	while (s[i])
	{
		update_quote(s[i], &quote, has_quote);
		if (s[i] == '*' && quote == 0)
			*has_wildcard = 1;
		if (quote == 0 && (is_space(s[i]) || is_operator_char(s[i])))
			break ;
		i++;
	}
	if (quote != 0)
		return (-1);
	return (i);
}

int	add_word_token(t_lexer *lex, char *s, int *i)
{
	int		start;
	int		end;
	int		has_quote;
	int		has_wildcard;
	char	*value;

	start = *i;
	has_quote = 0;
	has_wildcard = 0;
	end = scan_word_end(s, start, &has_quote, &has_wildcard);
	if (end == -1)
	{
		lex->err = LEX_ERR_UNCLOSED_QUOTE;
		return (0);
	}
	value = ms_substr(s, start, end - start);
	if (!value)
	{
		lex->err = LEX_ERR_MALLOC;
		return (0);
	}
	*i = end;
	return (add_token(lex, TOKEN_WORD, value, has_quote, has_wildcard));
}

