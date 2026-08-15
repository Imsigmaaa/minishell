/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 13:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/16 03:24:21 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
		free(argv[i++]);
	free(argv);
}

static int	has_equal(char *word)
{
	int	i;

	i = 0;
	while (word && word[i])
	{
		if (word[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

static int	add_word_fields(t_fields *output, char *word, t_shell *shell,
	int split)
{
	t_expbuf	buffer;
	t_fields	fields;
	int			i;

	if (!expbuf_init(&buffer))
		return (0);
	if (!build_expanded_word(word, shell, &buffer))
		return (expbuf_free(&buffer), 0);
	if ((split && !fields_from_buffer(&fields, &buffer))
		|| (!split && !fields_whole_buffer(&fields, &buffer)))
		return (expbuf_free(&buffer), 0);
	expbuf_free(&buffer);
	i = -1;
	while (++i < fields.count)
		if (!fields_add(output, fields.items[i], 0,
				ft_strlen(fields.items[i])))
			return (fields_free(&fields), 0);
	fields_free(&fields);
	return (1);
}

static int	expand_cmd_argv(t_shell *shell, t_cmd *cmd)
{
	t_fields	output;
	int			i;
	int			split;

	if (!fields_init(&output))
		return (0);
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		split = 1;
		if (output.count && env_strcmp(output.items[0], "export") == 0
			&& has_equal(cmd->argv[i]))
			split = 0;
		if (!add_word_fields(&output, cmd->argv[i], shell, split))
			return (fields_free(&output), 0);
		i++;
	}
	free_argv(cmd->argv);
	cmd->argv = output.items;
	return (1);
}

int	expand_all_argv(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = shell->cmds;
	while (cmd)
	{
		if (!expand_cmd_argv(shell, cmd))
			return (0);
		cmd = cmd->next;
	}
	return (1);
}
