/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 17:19:10 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include <dirent.h>
# include <fnmatch.h>
# include "minishell.h"

void	expand_wildcards(t_group *group, int index);
int		remove_quotes(char *str);
/*	expander_utils.c	*/
char	*get_value(t_group *group, char *word);
int		get_keylen(char *word);
void	remove_first_char(char *str);
void	remove_redirect(char **cmd);
void	remove_word(char **tab, int index);
int		insert_word(t_group *group, char *new_word, int index);

#endif