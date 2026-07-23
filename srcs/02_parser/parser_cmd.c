/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 00:34:51 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/23 16:35:17 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd *cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		ft_free_arr(cmds->argv);
		ft_free_t_redir(cmds->redirs);
		free(cmds);
		cmds = tmp;
	}
}

static	void	append_value(char **tmp, char *value, char **av, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{	
		tmp[i] = av[i];
		i ++;
	}
	tmp[i] = value;
	tmp[i + 1] = NULL;
	return ;
}

int	add_arg(t_cmd *cmd, t_token *token)
{
	char **tmp;
	char *value;
	size_t	size;
	
	value = ft_strdup(token->value);
	if (!value)
		return (perror("malloc"), 0);
	size = ft_arrlen(cmd->argv);
	tmp = malloc(sizeof(char *) * (size + 2));
	if (!tmp)
		return (free(value),perror("malloc"), 0);
	append_value(tmp, value, cmd->argv, size);
	free(cmd->argv);
	cmd->argv = tmp;
	return (1);
}