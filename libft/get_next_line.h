/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 11:05:54 by xingchen          #+#    #+#             */
/*   Updated: 2026/04/19 18:49:40 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
# include <stddef.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>

void	gnl_memcpy(char *dup, char *s, size_t n);
char	*get_next_line(int fd);
char	*free_buffer(char *buffer);
int		is_line_in_buffer(char *buffer);
#endif