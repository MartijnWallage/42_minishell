/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:09:10 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/03 13:29:59 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool    is_whitespace(const char c)
{
    if (c == ' ')
        return (true);
    if (c == '\t')
        return (true);
    if (c == '\v')
        return (true);
    if (c == '\n')
        return (true);
    return (false);
}

bool    is_special_char(const char c)
{
    if (c == '|')
        return (true);
    if (c == '<')
        return (true);
    if (c == '>')
        return (true);
    if (c == '(')
        return (true);
    if (c == ')')
        return (true);
    if (c == '&')
        return (true);
    return (false);
}

bool	is_special_charpair(const char a, const char b)
{
	if ((a == b)
        && (a == '&' || a == '|' || a == '<' || a == '>'))
		return (true);
	return (false);
}

bool	is_quotation_mark(const char c)
{
	if (c == '\'' || c == '\"')
		return (true);
	return (false);
}

int	wordlen(const char *str, const char c)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	return (len);
}
