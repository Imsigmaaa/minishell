/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 10:14:00 by xingchen          #+#    #+#             */
/*   Updated: 2026/03/20 12:57:35 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*add_back(void *temp, t_list **head)
{
	t_list	*node;
	t_list	*map;

	node = ft_lstnew(temp);
	if (!node)
		return (NULL);
	if (!(*head))
	{
		*head = node;
		return (node);
	}
	map = *head;
	while (map->next)
		map = map->next;
	map->next = node;
	return (node);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_head;
	t_list	*new_node;
	void	*temp;

	new_head = NULL;
	if (!lst || !f)
		return (NULL);
	while (lst)
	{
		temp = f(lst->content);
		if (!temp)
		{
			ft_lstclear(&new_head, del);
			return (NULL);
		}
		new_node = add_back(temp, &new_head);
		if (!new_node)
		{
			del(temp);
			ft_lstclear(&new_head, del);
			return (NULL);
		}
		lst = lst->next;
	}
	return (new_head);
}
