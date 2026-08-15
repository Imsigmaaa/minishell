/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_add_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 23:51:30 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 12:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_quote(char c, char *quote, int *has_quote)
{
	if ((c == '\'' || c == '"') && *quote == 0)
	{
		*quote = c;
		*has_quote = 1;
	}
	else if (c == *quote)
		*quote = 0;
}

static int	scan_word_end(char *input, int start, int *has_quote)
{
	int		i;
	char	quote;

	i = start;
	quote = 0;
	while (input[i])
	{
		update_quote(input[i], &quote, has_quote);
		if (quote == 0 && (is_space(input[i])
				|| is_operator_char(input[i])))
			break ;
		i++;
	}
	if (quote != 0)
		return (-1);
	return (i);
}

int	add_word_token(t_lexer *lex, char *input, int *index)
{
	int		start;
	int		end;
	int		has_quote;
	char	*value;

	start = *index;
	has_quote = 0;
	end = scan_word_end(input, start, &has_quote);
	if (end == -1)
	{
		lex->err = LEX_ERR_UNCLOSED_QUOTE;
		return (0);
	}
	value = ms_substr(input, start, end - start);
	if (!value)
	{
		lex->err = LEX_ERR_MALLOC;
		return (0);
	}
	*index = end;
	return (add_token(lex, TOKEN_WORD, value, has_quote));
}
