/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/09 17:16:42 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define FORMAT	"philoshell: format\n\t./minishell [no arguments]"
# define PROMPT "philoshell 🤔> "
# define INVALID_INPUT	"philoshell: input is invalid"

# define PIPE	1
# define AND	2
# define OR		3

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
# include <sys/wait.h>
# include "../libft/inc/libft.h"

typedef struct s_branch
{
	char			*command;
	char			connector;
	int				pipefd[2];
	int				infile;
	int				outfile;
	int				errfile;
	struct s_branch	*left;
	struct s_branch	*right;
}					t_branch;

extern char	**genv;

int			pipex(int ac, char **av, char **env);
void		handle_error(char *info, int exitcode);
void		free_tab(char **tab);
char		*get_path(char *cmd, char **env);
void		exec(char *cmd, char **env);
t_branch	*init_branch(char *command);
void		read_tree(t_branch *branch);
void		simple_command(t_branch *branch);

#endif
