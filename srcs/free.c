/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 17:07:04 by xingchen          #+#    #+#             */
/*   Updated: 2026/06/09 17:07:52 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_arr(char **arr)
{
	int	i;

	i = 0;
	if(!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_tokens(t_token *tokens)
{
	t_token *tmp;
	
	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}
