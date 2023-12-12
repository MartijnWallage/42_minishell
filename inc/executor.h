/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 10:33:19 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

/*	exec_pipe.c		*/
void	pipeline(t_group *group);
void	child(t_group *group);
/*	exec_redirect.c	*/
int		redirect(t_group *group);
void	restore_redirection(t_group *group);
/*	exec_utils.c	*/
char	*try_paths(char *cmd, char **paths);
char	*get_path(char *cmd, char **env);
void	ft_waitpid(t_group *group);
int		ft_dup2(t_group *group, int fd1, int fd2);
t_group	*next_complete_command(t_group *group);
/*	executor.c		*/
void	exec(t_group *group);
void	simple_command(t_group *group);
void	open_subshell(t_group *group);
/*	exec_heredoc.c	*/
int		handle_heredoc(t_group *group, char *eof);
void	write_heredoc(t_group *group, char *eof, int pipefd[2]);

#endif