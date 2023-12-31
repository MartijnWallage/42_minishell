/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2024/01/06 22:04:14 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef HOSTNAME
#  define HOSTNAME		"host"
# endif

# define FORMAT			"philoshell: format\n\t./minishell [no arguments]"
# define INVALID_INPUT	"philoshell: input is invalid"
# define PIPE_ERR		"pipe error"
# define FORK_ERR		"fork error"
# define MALLOC_MSG		"malloc error"
# define MALLOC_CODE	142

# define GRN_BOLD		"\001\033[1;32m\002"
# define BLU_BOLD		"\001\033[1;34m\002"
# define RES			"\001\033[0m\002"

# include <errno.h>
# include <string.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <unistd.h>
# include <stddef.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/wait.h>
# include "../libft/inc/libft.h"

extern volatile sig_atomic_t	g_sigid;

typedef enum s_operator
{
	NONE,
	PIPE,
	AND,
	OR,
	OPEN_SUBSHELL,
	CLOSE_SUBSHELL
}	t_operator;

typedef enum s_mode
{
	NON_INTERACTIVE,
	INTERACTIVE,
	HEREDOC
}	t_mode;

typedef struct s_group
{
	char			**cmd;
	char			***env_ptr;
	t_operator		operator;
	int				pipefd[2];
	pid_t			pid;
	int				original_stdin;
	int				original_stdout;
	int				heredoc;
	int				infile;
	int				outfile;
	int				*exitcode;
	struct s_group	*subshell;
	struct s_group	*previous;
	struct s_group	*next;
}					t_group;

/*	reader.c		*/
char	*reader(char **env);
/*	tokenizer		*/
char	**tokenizer(char const *s);
int		is_token(const char *token);
/*	parser		*/
int		is_valid_syntax(char **cmd, int *exitcode);
t_group	*parser(char **tokens, char ***env_ptr, int *exitcode);
int		parse_heredoc(t_group *group);
/*	expander	*/
void	expander(t_group *list);
void	expand_heredocs(t_group *group);
int		insert_word(t_group *group, char *new_word, int index);
/*	exec			*/
void	executor(t_group *group);
int		ft_dup2(t_group *group, int fd1, int fd2);
void	ft_waitpid(t_group *group);
void	*restore_redirection(t_group *group);
/*	builtin			*/
int		builtin(t_group *group);
/*	error.c			*/
int		error_msg(const char *info);
int		syntax_error(const char *token, int *exitcode);
int		redirect_error(t_group *group, const char *info);
/*	utils_tab.c	*/
int		tab_len(char **tab);
char	**copy_tab(char **tab);
void	*free_tab(char **tab);
void	remove_word(char **tab, int index);
/*	utils.c			*/
char	*mini_getenv(char **env, char *key);
t_group	*group_last(t_group *group);
/* utils_checks. */
int		is_redirect(const char *str);
int		is_whitespace(const char c);
int		is_meta_char(const char c);
int		is_control_operator(const char *token);
int		is_quotation_mark(const char c);
/*	clean.c			*/
void	cleanup(t_group *list);
void	cleanup_and_exit(t_group *list, int exitcode);
void	protect_malloc(t_group *group, void *ptr);
void	protect_malloc_during_build(char **cmd, char ***env_ptr, void *ptr);
void	*protect_and_free(t_group *group, void *ptr, char **tab, char *str);
/* signals.c */
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	handle_signals(t_mode mode);

#endif
