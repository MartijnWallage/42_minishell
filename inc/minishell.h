/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/16 15:59:47 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define FORMAT	"philoshell: format\n\t./minishell [no arguments]"
# define PROMPT "philoshell 🤔> "
# define PROMPT2 "\n partyshell 😝💃😝🕺👽> "
# define INVALID_INPUT	"philoshell: input is invalid"
# define PIPE			1
# define AND			2
# define OR				3
# define PIPE_ERR		"pipe error"
# define FORK_ERR		"fork error"
# define MALLOC_MSG		"malloc error"
# define MALLOC_CODE	142

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
# include <time.h> // delete

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
	int				infd;
	int				outfd;
	struct s_group	*previous;
	struct s_group	*next;
}					t_group;

/*	reader			*/
	/*	reader.c	*/
char	*reader(const char *env);
const char *prompt(char **str);

/*	lexer			*/
char	**lexer(char *line);

/*	parser		*/
	/*	parser.c	*/
t_group	*parser(char **tokens, char **env);

/*	expander	*/
void	expander(t_group *list);
/*	exec			*/
	/*	executor.c	*/
void	simple_command(t_group *node);
void	executor(t_group *group);
	/*	exec.c		*/
void	exec(char **cmd, char **env);
/*	builtin			*/
bool	is_builtin(char *cmd);
void	builtin(t_group *group);
void	builtin_export(t_group *group);
void	builtin_unset(t_group *group);
void	builtin_cd(t_group *group);
/*	error			*/
	/*	error.c		*/
void	handle_error(char *info, int exitcode);
/*	utils			*/
	/*	utils.c		*/
int		tab_len(char **tab);
void	*free_tab(char **tab);
char	**copy_tab(char **tab);
int		key_compare(char **env, char *line);
char	*get_key(char *str);
char	*get_value(char *str);
char	*ft_strjoin_safe(char const *s1, char const *s2);

	/* !utils.c*/
int		needle_check(char *str, char c);
	
/*	clean			*/
	/*	clean.c		*/
void	free_list(t_group *list);
void	cleanup(t_group *list);

#endif
