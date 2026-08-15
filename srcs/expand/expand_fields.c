/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_fields.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 13:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/16 02:44:46 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

int	fields_init(t_fields *fields)
{
	fields->items = malloc(sizeof(char *));
	if (!fields->items)
		return (0);
	fields->items[0] = NULL;
	fields->count = 0;
	return (1);
}

void	fields_free(t_fields *fields)
{
	int	i;

	i = 0;
	while (fields->items && fields->items[i])
		free(fields->items[i++]);
	free(fields->items);
	fields->items = NULL;
	fields->count = 0;
}

int	fields_add(t_fields *fields, char *text, int start, int length)
{
	char	**new_items;
	char	*value;
	int		i;

	value = ft_substr(text, start, length);
	if (!value)
		return (0);
	new_items = malloc(sizeof(char *) * (fields->count + 2));
	if (!new_items)
		return (free(value), 0);
	i = -1;
	while (++i < fields->count)
		new_items[i] = fields->items[i];
	new_items[i++] = value;
	new_items[i] = NULL;
	free(fields->items);
	fields->items = new_items;
	fields->count++;
	return (1);
}

int	fields_from_buffer(t_fields *fields, t_expbuf *buffer)
{
	int	i;
	int	start;

	if (!fields_init(fields))
		return (0);
	i = 0;
	while (i < buffer->len)
	{
		while (i < buffer->len && !buffer->quoted[i]
			&& is_space(buffer->text[i]))
			i++;
		start = i;
		while (i < buffer->len && (buffer->quoted[i]
				|| !is_space(buffer->text[i])))
			i++;
		if (i > start && !fields_add(fields, buffer->text, start, i - start))
			return (fields_free(fields), 0);
	}
	if (fields->count == 0 && buffer->keep_empty
		&& !fields_add(fields, "", 0, 0))
		return (fields_free(fields), 0);
	return (1);
}

int	fields_whole_buffer(t_fields *fields, t_expbuf *buffer)
{
	if (!fields_init(fields))
		return (0);
	if ((buffer->len > 0 || buffer->keep_empty)
		&& !fields_add(fields, buffer->text, 0, buffer->len))
		return (fields_free(fields), 0);
	return (1);
}
