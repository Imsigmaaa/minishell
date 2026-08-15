/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 13:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 13:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_INTERNAL_H
# define EXPAND_INTERNAL_H

# include "minishell.h"

typedef struct s_expbuf
{
	char			*text;
	unsigned char	*quoted;
	int				len;
	int				quote;
	int				keep_empty;
}	t_expbuf;

typedef struct s_fields
{
	char	**items;
	int		count;
}	t_fields;

typedef struct s_new_buffer
{
	char			*text;
	unsigned char	*quoted;
	int				length;
}	t_new_buffer;

typedef struct s_expand_ctx
{
	char		*word;
	int			pos;
	t_shell		*shell;
	t_expbuf	*buffer;
}	t_expand_ctx;

int		expbuf_init(t_expbuf *buffer);
void	expbuf_free(t_expbuf *buffer);
int		expbuf_add_string(t_expbuf *buffer, char *value, int quoted);
int		expbuf_add_char(t_expbuf *buffer, char character, int quoted);
int		expand_dollar(t_expand_ctx *context);
int		build_expanded_word(char *word, t_shell *shell, t_expbuf *buffer);
int		word_has_quote(char *word);
char	*remove_quotes_only(char *word);
int		fields_init(t_fields *fields);
void	fields_free(t_fields *fields);
int		fields_add(t_fields *fields, char *text, int start, int length);
int		fields_from_buffer(t_fields *fields, t_expbuf *buffer);
int		fields_whole_buffer(t_fields *fields, t_expbuf *buffer);
int		expand_all_argv(t_shell *shell);
int		expand_all_redirs(t_shell *shell);

#endif
