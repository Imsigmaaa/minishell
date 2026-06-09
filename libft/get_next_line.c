/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 11:04:27 by xingchen          #+#    #+#             */
/*   Updated: 2026/04/19 18:46:58 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	char	*join_temp_buf_and_buffer(char *buffer, char *temp_buf)
{
	size_t	buffer_len;
	size_t	temp_buf_len;
	char	*temp;

	if (!buffer || buffer[0] == '\0')
		buffer_len = 0;
	else
		buffer_len = ft_strlen(buffer);
	temp_buf_len = ft_strlen(temp_buf);
	temp = malloc(buffer_len + temp_buf_len + 1);
	if (!temp)
	{
		free(buffer);
		return (NULL);
	}
	gnl_memcpy(temp, buffer, buffer_len);
	gnl_memcpy(temp + buffer_len, temp_buf, temp_buf_len);
	temp[buffer_len + temp_buf_len] = '\0';
	free(buffer);
	return (temp);
}

static	char	*cpy_line(char *buffer)
{
	size_t	i;
	char	*line;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i ++;
	if (buffer[i] == '\n')
		i ++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	gnl_memcpy(line, buffer, i);
	line[i] = '\0';
	return (line);
}

static	char	*delete_line_in_buffer(char *line, char *buffer)
{
	char	*new_buffer;
	size_t	line_len;
	size_t	buffer_len;

	line_len = ft_strlen(line);
	buffer_len = ft_strlen(buffer);
	if (line_len == buffer_len)
	{
		free (buffer);
		return (NULL);
	}
	new_buffer = malloc(buffer_len - line_len + 1);
	if (!new_buffer)
	{
		free (buffer);
		return (NULL);
	}
	gnl_memcpy(new_buffer, buffer + line_len, buffer_len - line_len);
	new_buffer[buffer_len - line_len] = '\0';
	free(buffer);
	return (new_buffer);
}

static	char	*read_file(int fd, char *buffer, ssize_t *len_temp_buf,
			int *has_newline)
{
	char	*temp_buf;

	temp_buf = malloc(BUFFER_SIZE + 1);
	if (!temp_buf)
	{
		free(buffer);
		return (NULL);
	}
	*len_temp_buf = read(fd, temp_buf, BUFFER_SIZE);
	if (*len_temp_buf == -1)
	{
		free(temp_buf);
		free(buffer);
		return (NULL);
	}
	temp_buf[*len_temp_buf] = '\0';
	*has_newline = is_line_in_buffer(temp_buf);
	if (*len_temp_buf > 0)
		buffer = join_temp_buf_and_buffer(buffer, temp_buf);
	free(temp_buf);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer = NULL;
	char		*line;
	int			has_newline;
	ssize_t		len_temp_buf;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	len_temp_buf = 1;
	has_newline = is_line_in_buffer(buffer);
	while (len_temp_buf > 0 && !has_newline)
		buffer = read_file(fd, buffer, &len_temp_buf, &has_newline);
	if (!buffer || !buffer[0])
	{
		buffer = free_buffer(buffer);
		return (NULL);
	}
	line = cpy_line(buffer);
	if (!line)
	{
		buffer = free_buffer(buffer);
		return (NULL);
	}
	buffer = delete_line_in_buffer(line, buffer);
	return (line);
}

/*#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>   // open
#include <unistd.h>

int main(void)
{
    int fd;
    char *line;
	size_t i;

    // 打开文件
    fd = open("test.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("打开文件失败！\n");
        return 1;
    }

    // 调用 get_next_line 只读取一次
    line = get_next_line(fd);
	 i = 0;
    while (line[i] != '\0')
    {
        printf("%d ", line[i]);
        i++;
    }
    printf("\nlen = %zu\n", i);
    printf("%s", line);
    free(line);

    close(fd);
    return 0;
}*/
