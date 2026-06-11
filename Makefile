# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/11 21:14:31 by xingchen          #+#    #+#              #
#    Updated: 2026/06/11 22:47:29 by xingchen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#可执行文件名
NAME = minishell
#编译器
CC = cc
#编译选项
CFLAGS = -Wall -Wextra -Werror
#头文件目录
INCLUDES = -Iincludes -Ilibft

LIBFT = libft/libft.a
#源文件
SRCS = srcs/main.c \
       srcs/free.c \
       srcs/utils1.c \
	   srcs/01_lexer/lexer.c \
       srcs/01_lexer/lexer_add_operator.c \
       srcs/01_lexer/lexer_add_word.c \
       srcs/01_lexer/lexer_error.c \
       srcs/01_lexer/lexer_token.c \
       srcs/01_lexer/lexer_utils.c \
       srcs/02_parser/parser.c \
       srcs/02_parser/parser_cmd.c \
       srcs/02_parser/parser_redir.c \
       srcs/02_parser/parser_syntax.c
#自动把.c变成.o
OBJS = $(SRCS:.c=.o)
#m默认规则
all: $(NAME)
$(LIBFT):
	make -C libft
#链接所有.o生成minishell
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
#编译.c->.o
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
#删除目标文件
clean:
	rm -f $(OBJS)
	make -C libft clean
#删除目标文件和可执行文件
fclean: clean
	rm -f $(NAME)
	make -C libft fclean
#完全重新编译
re: fclean all
#声明伪目标
.PHONEY: all clean fclean re

