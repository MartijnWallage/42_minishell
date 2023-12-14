/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 17:13:40 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/*	parser_utils.c */
int			find_closing_parenth(char **cmd);
int			first_operator(char **tokens);
t_operator	get_operator(char *str);
/*	syntax_utils.c	*/
int			check_left_arg(char **cmd, int index, int *exitcode);
int			check_right_arg(char **cmd, int index, int *exitcode);

#endif