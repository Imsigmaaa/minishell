/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 13:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 13:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

int	word_has_quote(char *word)
{
	int	i;

	i = 0;
	while (word && word[i])
	{
		if (word[i] == '\'' || word[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

static int	process_word_char(t_expand_ctx *context, char *quote)
{
	if (!*quote && (context->word[context->pos] == '\''
			|| context->word[context->pos] == '"'))
	{
		*quote = context->word[context->pos++];
		context->buffer->keep_empty = 1;
	}
	else if (*quote && context->word[context->pos] == *quote)
	{
		*quote = 0;
		context->pos++;
	}
	else if (context->word[context->pos] == '$' && *quote != '\'')
	{
		context->buffer->quote = *quote;
		return (expand_dollar(context));
	}
	else if (!expbuf_add_char(context->buffer,
			context->word[context->pos++], *quote != 0))
		return (0);
	return (1);
}

int	build_expanded_word(char *word, t_shell *shell, t_expbuf *buffer)
{
	t_expand_ctx	context;
	char			quote;

	context.word = word;
	context.pos = 0;
	context.shell = shell;
	context.buffer = buffer;
	quote = 0;
	while (word[context.pos])
		if (!process_word_char(&context, &quote))
			return (0);
	return (1);
}

char	*remove_quotes_only(char *word)
{
	t_expbuf	buffer;
	char		quote;
	int			i;

	if (!expbuf_init(&buffer))
		return (NULL);
	quote = 0;
	i = 0;
	while (word[i])
	{
		if (!quote && (word[i] == '\'' || word[i] == '"'))
			quote = word[i++];
		else if (quote && word[i] == quote)
		{
			quote = 0;
			i++;
		}
		else if (!expbuf_add_char(&buffer, word[i++], 1))
			return (expbuf_free(&buffer), NULL);
	}
	free(buffer.quoted);
	return (buffer.text);
}
