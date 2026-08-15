/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 12:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/16 00:46:17 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <signal.h>
#include "libft.h"
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	int					has_quote;
	struct s_token		*next;
}	t_token;

typedef enum e_lex_error
{
	LEX_OK,
	LEX_ERR_MALLOC,
	LEX_ERR_UNCLOSED_QUOTE,
	LEX_ERR_UNSUPPORTED_TOKEN
}	t_lex_error;

typedef struct s_lexer
{
	t_token	*head;
	t_token	*tail;
	int		err;
}	t_lexer;

typedef struct s_redir
{
	t_token_type		type;
	char				*target;
	int					heredoc_fd;
	int					heredoc_expand;
	struct s_redir		*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env		*env;
	t_token		*tokens;
	t_cmd		*cmds;
	int			exit_status;
	int			should_exit;
}	t_shell;

typedef struct s_exec
{
	int			cmd_count;
	int			(*pipe_fd)[2];
	pid_t		*pids;
	int			status;
}	t_exec;

extern volatile sig_atomic_t	g_signal;

int			is_space(char c);
int			is_operator_char(char c);
int			add_word_token(t_lexer *lex, char *input, int *index);
int			add_operator_token(t_lexer *lex, char *input, int *index);
int			add_token(t_lexer *lex, t_token_type type, char *value,
				int has_quote);
t_token		*lexer(char *input, int *err);
void		print_lexer_error(int err);
char		*ms_strdup(char *str);
char		*ms_substr(char *str, int start, int len);
size_t		ms_strlen(char *str);
void		free_tokens(t_token **tokens);

int			syntax_check(t_token *tokens);
int			syntax_error(char *token);
t_cmd		*parse_tokens(t_token *tokens);
t_cmd		*new_cmd(void);
int			add_arg(t_cmd *cmd, t_token *token);
int			is_redir_token(t_token_type type);
int			add_redir(t_cmd *cmd, t_token *token);
void		free_cmds(t_cmd *cmds);

int			env_strlen(char *str);
int			env_strcmp(char *str1, char *str2);
char		*env_join_pair(char *key, char *value);
t_env		*env_new(char *key, char *value, int exported);
void		env_add_back(t_env **env, t_env *new_node);
void		env_free(t_env **env);
t_env		*env_init(char **envp);
t_env		*env_find(t_env *env, char *key);
char		*env_get(t_env *env, char *key);
int			env_set(t_env **env, char *key, char *value, int exported);
int			env_unset(t_env **env, char *key);
char		**env_to_array(t_env *env);
void		env_free_array(char **array);
int			env_is_valid_key(char *key);
void		env_print(t_env *env);
void		env_print_export(t_env *env);

int			executor(t_shell *shell);
void		exec_single(t_shell *shell);
void		exec_pipe(t_shell *shell);
void		exec_cmd(t_cmd *cmd, t_env *env);
int			candidate_status(char *candidate);
int			is_builtin(t_cmd *cmd);
int			exec_builtin(t_shell *shell, t_cmd *cmd);
int			init_exec_data(t_shell *shell, t_exec *exec);
int			create_pipes(t_shell *shell, t_exec *exec);
void		close_created_fd(t_exec *exec, int created_pipes);
void		free_exec(t_exec *exec);
void		update_exit_status(t_shell *shell, int status);
int			count_cmds(t_cmd *cmds);
void		wait_all_children(t_shell *shell, t_exec *exec, int count);
void		print_exec_error(char *command, int error);
int			exec_redir(t_cmd *cmd);
int			prepare_all_heredocs(t_shell *shell);
void		close_all_heredoc_fds(t_shell *shell);
char		*make_heredoc_path(int number);
int			open_unique_heredoc(char **path, int number);
int			finish_heredoc(t_shell *shell, t_redir *redir, char *path,
				int status);
int			write_heredoc_output(int fd, char *output);
char		*expand_heredoc_line(char *line, t_shell *shell);

int			expansion(t_shell *shell);
int			builtin_echo(t_cmd *cmd);
int			builtin_cd(t_shell *shell, t_cmd *cmd);
int			builtin_pwd(t_cmd *cmd);
int			builtin_export(t_shell *shell, t_cmd *cmd);
int			builtin_unset(t_shell *shell, t_cmd *cmd);
int			builtin_env(t_shell *shell, t_cmd *cmd);
int			builtin_exit(t_shell *shell, t_cmd *cmd);
void		print_export_sorted(t_env *env);
int			init_interactive_signals(void);
void		sync_signal_status(t_shell *shell);

int			init_shell(t_shell *shell, char **envp);
void		cleanup_shell(t_shell *shell);
void	ft_free_arr(char **arr);
size_t	ft_arrlen(char **arr);
void	ft_free_t_redir(t_redir *redirs);
int	ft_strcmp(const char *s1, const char *s2);
void	print_child_signal(int status);
char		*get_heredoc_path(int index);
pid_t	run_heredoc_child(t_shell *shell, t_redir *redir, int fd);
int		wait_heredoc_child(pid_t pid, int *status);
void		remove_heredoc_file(char *path);
void		print_heredoc_warning(char *delimiter);
int	init_heredoc_signals(void);
int	open_heredoc_read(t_shell *shell, t_redir *redir, char *path);
int	open_heredoc_file(char **path, int number);

#endif
