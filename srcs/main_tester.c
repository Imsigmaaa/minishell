/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tester.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 17:11:13 by xingchen          #+#    #+#             */
/*   Updated: 2026/06/09 17:31:48 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../../libft/libft.h"
void print_cmds(t_cmd *cmds)
{
	t_cmd *tmp;

	while (tmp)
	{
		int i = 0;
		while (cmds->argv[i])
		{
			printf("%s\n",cmds->argv[i]);
			i ++;
		}
		t_redir *redir = cmds->redirs;
		while (redir)
		{
			printf("%d\n",redir->type);
			printf("%s\n",redir->target);
			redir = redir->next;	
		}
		tmp = tmp->next;	
	}
}
int main(void)
{
	t_token t1;
	t_token t2;
	t_token t3;
	t_token t4;
	t_cmd	*cmds;	
	t1.type = TOKEN_WORD;
	t1.value = "echo";
	t1.next = &t2;
	
	t2.type = TOKEN_WORD;
	t2.value = "\"hello\"";
	t2.next = &t3;

	t3.type = TOKEN_REDIR_OUT;
	t3.value = ">";
	t3.next = &t4;

	t4.type = TOKEN_WORD;
	t4.value = "file";
	t4.next = NULL;

	cmds = parse_tokens(&t1);

	print_cmds(cmds);
	free_cmds(cmds);

}