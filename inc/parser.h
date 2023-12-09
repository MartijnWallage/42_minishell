/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:40:59 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/08 22:57:13 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "minishell.h"

/*	parser_utils.c */
int		find_closing_parenth(char **cmd);
int		first_operator(char **tokens);
char	**get_left_side(char **tab, int end);
char	**get_right_side(char **tab, int begin);
t_operator	get_operator(char *str);
/*	syntax_check	*/
int		is_valid_syntax(char **cmd, int *exitcode);

#endif