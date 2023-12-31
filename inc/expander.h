/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2024/01/06 21:27:07 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include <dirent.h>
# include <fnmatch.h>
# include "minishell.h"

void	expand_wildcards(t_group *group, int index);
char	*expand_var(t_group *group, char *old_line, int *dollar_sign);
/*	expander_utils.c	*/
char	*get_value(t_group *group, char *word);
int		get_keylen(char *word);
/*	expander_remove.c	*/
int		remove_quotes(char *str);
void	remove_redirect(char **cmd);
void	remove_first_char(char *str);

#endif