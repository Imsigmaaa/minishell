/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 13:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 13:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static int	is_var_char(char character)
{
	return ((character >= 'a' && character <= 'z')
		|| (character >= 'A' && character <= 'Z')
		|| (character >= '0' && character <= '9') || character == '_');
}

static char	*status_to_string(int status)
{
	char	digits[12];
	char	*value;
	int		i;
	int		length;

	if (status < 0)
		status = 0;
	digits[11] = '\0';
	i = 11;
	while (status > 0)
	{
		digits[--i] = '0' + status % 10;
		status /= 10;
	}
	if (i == 11)
		digits[--i] = '0';
	length = 11 - i;
	value = ms_substr(digits, i, length);
	return (value);
}

static int	expand_status(t_expand_ctx *context)
{
	char	*value;
	int		result;

	value = status_to_string(context->shell->exit_status);
	if (!value)
		return (0);
	result = expbuf_add_string(context->buffer, value,
			context->buffer->quote != 0);
	free(value);
	context->pos += 2;
	return (result);
}

static int	expand_named(t_expand_ctx *context, int start, int length)
{
	char	*key;
	char	*value;
	int		result;

	key = ms_substr(context->word, start, length);
	if (!key)
		return (0);
	value = env_get(context->shell->env, key);
	result = expbuf_add_string(context->buffer, value,
			context->buffer->quote != 0);
	free(key);
	context->pos = start + length;
	return (result);
}

int	expand_dollar(t_expand_ctx *context)
{
	int	start;
	int	length;

	if (context->word[context->pos + 1] == '?')
		return (expand_status(context));
	start = context->pos + 1;
	length = 0;
	while (is_var_char(context->word[start + length]))
		length++;
	if (length == 0)
	{
		context->pos++;
		return (expbuf_add_char(context->buffer, '$',
				context->buffer->quote != 0));
	}
	return (expand_named(context, start, length));
}
