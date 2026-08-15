/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 13:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/16 03:25:16 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static void	fill_new_buffer(t_expbuf *buffer, t_new_buffer *new,
	char *value, int quoted)
{
	int	i;

	i = -1;
	while (++i < buffer->len)
	{
		new->text[i] = buffer->text[i];
		new->quoted[i] = buffer->quoted[i];
	}
	while (i < buffer->len + new->length)
	{
		new->text[i] = value[i - buffer->len];
		new->quoted[i++] = quoted;
	}
	new->text[i] = '\0';
	new->quoted[i] = 0;
}

int	expbuf_init(t_expbuf *buffer)
{
	buffer->text = malloc(1);
	buffer->quoted = malloc(1);
	if (!buffer->text || !buffer->quoted)
	{
		free(buffer->text);
		free(buffer->quoted);
		return (0);
	}
	buffer->text[0] = '\0';
	buffer->quoted[0] = 0;
	buffer->len = 0;
	buffer->quote = 0;
	buffer->keep_empty = 0;
	return (1);
}

void	expbuf_free(t_expbuf *buffer)
{
	free(buffer->text);
	free(buffer->quoted);
	buffer->text = NULL;
	buffer->quoted = NULL;
	buffer->len = 0;
}

int	expbuf_add_string(t_expbuf *buffer, char *value, int quoted)
{
	t_new_buffer	new;

	new.length = ft_strlen(value);
	new.text = malloc(buffer->len + new.length + 1);
	new.quoted = malloc(buffer->len + new.length + 1);
	if (!new.text || !new.quoted)
		return (free(new.text), free(new.quoted), 0);
	fill_new_buffer(buffer, &new, value, quoted);
	free(buffer->text);
	free(buffer->quoted);
	buffer->text = new.text;
	buffer->quoted = new.quoted;
	buffer->len += new.length;
	return (1);
}

int	expbuf_add_char(t_expbuf *buffer, char character, int quoted)
{
	char	value[2];

	value[0] = character;
	value[1] = '\0';
	return (expbuf_add_string(buffer, value, quoted));
}
