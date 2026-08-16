NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -Iincludes -Isrcs/expand -Ilibft
LDLIBS = -Llibft -lft -lreadline
RM = rm -f

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_FILES = $(wildcard $(LIBFT_DIR)/*.c) \
			  $(wildcard $(LIBFT_DIR)/*.h) $(LIBFT_DIR)/Makefile

CORE_SRCS = srcs/main.c \
			srcs/init.c \

LEXER_SRCS = srcs/lexer/lexer.c \
			 srcs/lexer/lexer_add_operator.c \
			 srcs/lexer/lexer_add_word.c \
			 srcs/lexer/lexer_error.c \
			 srcs/lexer/lexer_token.c \
			 srcs/lexer/lexer_utils.c

PARSER_SRCS = srcs/parser/parser.c \
			  srcs/parser/parser_cmd.c \
			  srcs/parser/parser_redir.c \
			  srcs/parser/parser_syntax.c

EXPAND_SRCS = srcs/expand/expand.c \
			  srcs/expand/expand_buffer.c \
			  srcs/expand/expand_cmd.c \
			  srcs/expand/expand_fields.c \
			  srcs/expand/expand_redir.c \
			  srcs/expand/expand_var.c \
			  srcs/expand/expand_word.c

EXEC_SRCS = srcs/exec/exec_cmd.c \
			srcs/exec/exec_path.c \
			srcs/exec/exec_path_split.c \
			srcs/exec/exec_pipe.c \
			srcs/exec/exec_pipe_child.c \
			srcs/exec/exec_pipe_init.c \
			srcs/exec/exec_pipe_utils.c \
			srcs/exec/exec_redir.c \
			srcs/exec/exec_single.c \
			srcs/exec/execv.c

HEREDOC_SRCS = srcs/heredoc/heredoc.c \
			   srcs/heredoc/heredoc_child.c \
			   srcs/heredoc/heredoc_file.c

BUILTIN_SRCS = srcs/builtin/builtin.c \
			   srcs/builtin/cd.c \
			   srcs/builtin/echo.c \
			   srcs/builtin/env.c \
			   srcs/builtin/exit.c \
			   srcs/builtin/export.c \
			   srcs/builtin/export_print.c \
			   srcs/builtin/pwd.c \
			   srcs/builtin/unset.c

ENV_SRCS = srcs/env/env_array.c \
		   srcs/env/env_get.c \
		   srcs/env/env_init.c \
		   srcs/env/env_node.c \
		   srcs/env/env_print.c \
		   srcs/env/env_set.c \
		   srcs/env/env_unset.c \
		   srcs/env/env_utils.c \
		   srcs/env/env_valid.c

SIGNAL_SRCS = srcs/signal/signal.c

SRCS = $(CORE_SRCS) $(LEXER_SRCS) $(PARSER_SRCS) $(EXPAND_SRCS) \
	   $(EXEC_SRCS) $(HEREDOC_SRCS) $(BUILTIN_SRCS) $(ENV_SRCS) \
	   $(SIGNAL_SRCS)

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)

$(LIBFT): $(LIBFT_FILES)
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(EXPAND_SRCS:.c=.o): srcs/expand/expand_internal.h

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
