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

typedef struct s_branch
{
	char			**cmd;
	char			operator;
	char			**env;
	int				pipefd[2];
	bool			child;
	bool			lastborn;
	struct s_branch	*left;
	struct s_branch	*right;
}					t_branch;

/*	reader.c	*/
char		*reader(void);
/*	lexer.c		*/
char		**lexer(char *line);
/*	parser.c	*/
char		*get_path(char *cmd, char **env);
t_branch	*parser(char **tokens, char **envp);
/*	executor.c	*/
void		read_tree(t_branch *branch);
void		exec(char **cmd, char **env);
void		simple_command(t_branch *branch);
void		executor(t_branch *tree);
/*	pipex.c		*/
char		*get_path(char *cmd, char **env);
/*	error.c		*/
void		handle_error(char *info, int exitcode);
/*	utils.c		*/
size_t		tab_len(void **tab);
void		free_tab(char **tab);
/*	clean.c		*/
void		cleanup(t_branch *tree);

#endif
