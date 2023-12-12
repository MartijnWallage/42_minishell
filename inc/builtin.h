/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/05 11:00:11 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

int		builtin_export(t_group *group);
int		builtin_unset(t_group *group);
int		builtin_cd(t_group *group);
int		builtin_exit(t_group *group);
int		builtin_env(t_group *group);
int		builtin_pwd(t_group *group);
int		builtin_echo(t_group *group);
void	append_var(t_group *group, char *var);

#endif