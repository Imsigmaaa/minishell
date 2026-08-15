/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 13:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 13:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static void	replace_target(t_redir *redir, char *target)
{
	free(redir->target);
	redir->target = target;
}

static int	expand_file_redir(t_shell *shell, t_redir *redir)
{
	t_expbuf	buffer;
	t_fields	fields;

	if (!expbuf_init(&buffer))
		return (0);
	if (!build_expanded_word(redir->target, shell, &buffer)
		|| !fields_from_buffer(&fields, &buffer))
		return (expbuf_free(&buffer), 0);
	expbuf_free(&buffer);
	if (fields.count != 1)
	{
		write(2, "minishell: ", 11);
		write(2, redir->target, ms_strlen(redir->target));
		write(2, ": ambiguous redirect\n", 21);
		fields_free(&fields);
		shell->exit_status = 1;
		return (-1);
	}
	replace_target(redir, fields.items[0]);
	free(fields.items);
	return (1);
}

static int	expand_heredoc_delimiter(t_redir *redir)
{
	char	*delimiter;

	redir->heredoc_expand = !word_has_quote(redir->target);
	delimiter = remove_quotes_only(redir->target);
	if (!delimiter)
		return (0);
	replace_target(redir, delimiter);
	return (1);
}

int	expand_all_redirs(t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*redir;
	int		result;

	cmd = shell->cmds;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
				result = expand_heredoc_delimiter(redir);
			else
				result = expand_file_redir(shell, redir);
			if (result <= 0)
				return (result);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (1);
}

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	t_expand_ctx	context;
	t_expbuf		buffer;

	if (!expbuf_init(&buffer))
		return (NULL);
	context.word = line;
	context.pos = 0;
	context.shell = shell;
	context.buffer = &buffer;
	while (line[context.pos])
	{
		if (line[context.pos] == '$')
		{
			if (!expand_dollar(&context))
				return (expbuf_free(&buffer), NULL);
		}
		else if (!expbuf_add_char(&buffer, line[context.pos++], 0))
			return (expbuf_free(&buffer), NULL);
	}
	free(buffer.quoted);
	return (buffer.text);
}
