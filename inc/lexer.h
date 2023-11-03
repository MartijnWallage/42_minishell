/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:10:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/03 13:11:16 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

#include "minishell.h"

/*	lexer_utils.c	*/
bool	is_whitespace(const char c);
bool	is_special_char(const char c);
bool	is_special_charpair(const char a, const char b);
bool	is_quotation_mark(const char c);
int		wordlen(const char *str, const char c);

/*	tokenizer.c		*/
char	**tokenizer(char const *s);

#endif