/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 20:43:17 by mwallage         ###   ########.fr       */
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

# define GRN_BOLD		"\e[1;32m"
# define BLU_BOLD		"\e[1;34m"
# define RES			"\e[0m"

# include <errno.h>
# include <string.h>
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

typedef enum
{
	NONE,
	PIPE,
	AND,
	OR,
	OPEN_SUBSHELL,
	CLOSE_SUBSHELL
}	t_operator;

typedef struct s_group
{
	char			**cmd;
	char			***env_ptr;
 	t_operator		operator;
	int				pipefd[2];
	pid_t			pid;
	int				original_stdin;
	int				original_stdout;
	int				infile;
	int				outfile;
	int				*exitcode;
	struct s_group	*previous;
	struct s_group	*next;
}					t_group;

/*	reader.c		*/
char	*reader(char **env);
/*	tokenizer		*/
int		is_whitespace(const char c);
int		is_meta_char(const char c);
int		is_control_operator(const char *token);
int		is_token(const char *token);
int		is_quotation_mark(const char c);
int		wordlen(const char *str, const char c);
char	**tokenizer(char const *s);
/*	parser		*/
t_group	*parser(char **tokens, char ***env_ptr, int *exitcode);
/*	expander	*/
void	expander(t_group *list);
/*	exec			*/
void	executor(t_group *group);
/*	builtin			*/
int		builtin(t_group *group);
/*	error.c			*/
int		error_msg(const char *info);
int		syntax_error(const char *token, int *exitcode);
/*	utils.c			*/
int		tab_len(char **tab);
char	**copy_tab(char **tab);
char	*mini_getenv(char **env, char *key);
t_group	*group_last(t_group *group);
int		is_redirect(const char *str);
/*	clean.c			*/
void	*free_tab(char **tab);
void	cleanup(t_group *list);
void	cleanup_and_exit(t_group *list, int exitcode);
void	protect_malloc(t_group *group, void *ptr);
void	protect_malloc_during_build(char **cmd, char ***env_ptr, void *ptr);

#endif
