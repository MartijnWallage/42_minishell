/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/19 18:25:33 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/*	parser_utils.c */
char		**after_closing_parenth(char **cmd);
char		**get_next_operator(char **cmd);
char		**get_next_cmd(t_group *group, char **cmd);
char		**copy_tab_until_operator(char **tab);
t_operator	get_operator(char *str);
/*	syntax_utils.c	*/
int			check_left_arg(char **cmd, int index, int *exitcode);
int			check_right_arg(char **cmd, int index, int *exitcode);

#endif