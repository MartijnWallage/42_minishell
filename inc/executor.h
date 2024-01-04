/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/14 12:41:33 by mwallage         ###   ########.fr       */
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
/*	exec_utils.c	*/
char	*try_paths(t_group *group, char *cmd, char **paths);
char	*get_path(t_group *group, char *cmd, char **env);
t_group	*next_complete_command(t_group *group);
/*	executor.c		*/
void	exec(t_group *group);
void	*simple_command(t_group *group);
void	open_subshell(t_group *subshell, t_group *supershell);
/*	exec_heredoc.c	*/
int		open_heredoc(t_group *group);

#endif