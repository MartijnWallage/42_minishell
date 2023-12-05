/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/05 10:29:53 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

/*	executor.c		*/
void	exec(t_group *group);
void	simple_command(t_group *group);
/*	exec_redirect.c	*/
int		redirect(t_group *group);
/*	exec_utils.c	*/
char	*try_paths(char *cmd, char **paths);
char	*get_path(char *cmd, char **env);
void 	restore_redirection(t_group *group);

#endif