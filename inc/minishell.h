/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/24 17:06:52 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef HOSTNAME
#  define HOSTNAME		"host"
# endif

# define FORMAT			"philoshell: format\n\t./minishell [no arguments]"
# define INVALID_INPUT	"philoshell: input is invalid"
# define PIPE			1
# define AND			2
# define OR				3
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

typedef struct s_token
{
	char	*value;
	enum
	{
		TOKEN_WORD,
		TOKEN_PIPE,
		TOKEN_REDIRECT,
		TOKEN_HEREDOC,
		TOKEN_DOLLAR,
		TOKEN_VAR,
		TOKEN_AND,
		TOKEN_OR,
		TOKEN_SINGLE_QUOTE,
		TOKEN_DOUBLE_QUOTE,
		TOKEN_LPARENTH,
		TOKEN_RPARENTH,
	}	type;
}		t_token;

typedef struct s_group
{
	t_token			*token;
	char			**cmd;		// redundant, but easier for execve()
	char			**env;
	int				operator;	// Operator between this group and the next group
								// none (0), PIPE, or AND (&&), or OR (||)
	int				pipefd[2];
	char			*heredoc_delimiter;
	int				infile;
	int				outfile;
	int				exitcode;
	struct s_group	*previous;
	struct s_group	*next;
}					t_group;

/*	reader.c		*/
char	*reader(char **env);
/*	lexer			*/
char	**lexer(char *line);
char	**tokenizer(char const *s);
bool	is_whitespace(const char c);
bool	is_special_char(const char c);
bool	is_special_charpair(const char a, const char b);
bool	is_quotation_mark(const char c);
int		wordlen(const char *str, const char c);
/*	parser		*/
void	parse_redirect(t_group *group);
t_group	*parser(char **tokens, char **env, int exitcode);
/*	expander	*/
bool	is_end_of_key(char c);
void	remove_first_char(char *str);
void	expander(t_group *list);
/*	exec			*/
void	simple_command(t_group *node);
void	executor(t_group *group);
void	exec(char **cmd, char **env);
/*	builtin			*/
bool	is_builtin(char *cmd);
void	builtin(t_group *group);
void	builtin_export(t_group *group);
void	append_var(t_group *group, char *line);
void	builtin_unset(t_group *group);
void	builtin_cd(t_group *group);
/*	error.c			*/
void	err_and_exit(char *info, int exitcode);
void	err_no_exit(char *info);
/*	utils.c			*/
int		tab_len(char **tab);
void	*free_tab(char **tab);
char	**copy_tab(char **tab);
char	*mini_getenv(char **env, char *key);
int		key_compare(char **env, char *line);
char	*get_key(char *str);
char	*get_value(char *str);
char	*ft_strjoin_safe(char const *s1, char const *s2);
t_group	*group_last(t_group *group);
/*	clean.c			*/
void	free_list(t_group *list);
void	cleanup(t_group *list);

#endif
