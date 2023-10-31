/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/13 18:14:19 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define FORMAT	"philoshell: format\n\t./minishell [no arguments]"
# define PROMPT "philoshell 🤔> "
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

typedef struct s_token
{
	char	*value;
	enum
	{
		TOKEN_WORD,
		TOKEN_PIPE,
		TOKEN_SQUOTE,
		TOKEN_DQUOTE,
		TOKEN_LPARENTHESIS,
		TOKEN_RPARENTHESIS,
		TOKEN_VAR,
		TOKEN_OPERATOR,
	}	type;
}		t_token;

typedef struct s_group
{
	t_token			*token;
	char			**cmd;		// redundant, but easier for execve()
	char			**env;
	int				operator;	// will become redundant
	int				pipefd[2];
	int				infd;
	int				outfd;
	struct s_group	*previous;
	struct s_group	*next;
}					t_group;

/*	reader			*/
	/*	reader.c	*/
char	*reader(void);
/*	lexer			*/
	/*	lexer.c		*/
char	**lexer(char *line);
/*	parser		*/
	/*	parser.c	*/
t_group	*parser(char **tokens, char **env);
/*	exec			*/
	/*	executor.c	*/
void	simple_command(t_group *node);
void	executor(t_group *group);
	/*	exec.c		*/
void	exec(char **cmd, char **env);
/*	builtin			*/
bool	is_builtin(char *cmd);
void	builtin(t_group *group);
/*	error			*/
	/*	error.c		*/
void	handle_error(char *info, int exitcode);
/*	utils			*/
	/*	utils.c		*/
size_t	tab_len(void **tab);
void	free_tab(char **tab);
/*	clean			*/
	/*	clean.c		*/
void	free_list(t_group *list);
void	cleanup(t_group *list);

#endif
