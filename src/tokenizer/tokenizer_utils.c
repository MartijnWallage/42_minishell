/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:09:10 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/05 14:09:47 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_whitespace(const char c)
{
    if (c == ' ')
        return (1);
    else if (c == '\t')
        return (1);
    else if (c == '\n')
        return (1);
    return (0);
}

/*  A metacharacter is a character that, when unquoted, separates words. 
 *  A metacharacter is a space, tab, newline, or one of the following characters: 
 *  ‘|’, ‘&’, ‘;’ (not part of minishell), ‘(’, ‘)’, ‘<’, or ‘>’.   */
int    is_meta_char(const char c)
{
    if (c == ' ')
        return (1);
    else if (c == '\t')
        return (1);
    else if (c == '\n')
        return (1);
    else if (c == '|')
        return (1);
    else if (c == '&')
        return (1);
    else if (c == '(')
        return (1);
    else if (c == ')')
        return (1);
    else if (c == '<')
        return (1);
    else if (c == '>')
        return (1);
    return (0);
}

/*  A control operator is a token that performs a control function. 
 *  It is a newline or one of the following: ‘||’, ‘&&’, ‘&’, ‘;’, ‘;;’, ‘;&’, ‘;;&’,
 *  ‘|’, ‘|&’, ‘(’, or ‘)’. 
 *  For the purposes of minishell, we don't interpret ';' etc.   */
 
int    is_control_operator(const char *token)
{
    if (token[0] == '|' && token[1] == '|')
        return (2);
    else if (token[0] == '|')
        return (1);
    else if (token[0] == '&' && token[1] == '&')
        return (2);
    else if (token[0] == '(')
        return (1);
    else if (token[0] == ')')
        return (1);
    return (0);
}

int is_token(const char *token)
{
    if (token[0] == '|' && token[1] == '|')
        return (2);
    else if (token[0] == '|')
        return (1);
    else if (token[0] == '&' && token[1] == '&')
        return (2);
    else if (token[0] == '<' && token[1] == '<')
        return (2);
    else if (token[0] == '<')
        return (1);
    else if (token[0] == '>' && token[1] == '>')
        return (2);
    else if (token[0] == '>')
        return (1);
    else if (token[0] == '(')
        return (1);
    else if (token[0] == ')')
        return (1);
    return (0);
}

int	is_quotation_mark(const char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

int	wordlen(const char *str, const char c)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	return (len);
}
