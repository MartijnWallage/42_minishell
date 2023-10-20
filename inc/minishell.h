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
	char	*words;
	char	*meta;
	char	*metameta;
	bool	last_token;
}			t_token;

typedef struct s_branch
{
	token_t			*command;
	char			**env;
	int				pipefd[2];
	int				infile;
	int				outfile;
	int				errfile;
	bool			child;
	bool			last_born;
	struct s_branch	*left;
	struct s_branch	*right;
}					t_branch;

/*	reader.c	*/
char		*reader(void);
/*	lexer.c		*/
t_token		*lexer(char *line);
/*	parser.c	*/
t_branch	*init_branch(char *command);
char		*get_path(char *cmd, char **env);
t_branch	*parser(t_token *tokens, char **envp);
/*	executor.c	*/
void		read_tree(t_branch *branch);
void		exec(char *cmd, char **env);
void		simple_command(t_branch *branch);
void		executor(t_branch *tree);
/*	pipex.c		*/
char		*get_path(char *cmd, char **env);
/*	error.c		*/
void		handle_error(char *info, int exitcode);
/*	utils.c		*/
t_token		*token_cpy(t_token *tokens, int begin, int end);
size_t		tokens_len(t_token *tokens);
size_t		tab_len(void **tab);
void		free_tab(char **tab);
/*	clean.c		*/
void		cleanup(t_branch *tree)

#endif
