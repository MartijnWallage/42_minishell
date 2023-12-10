/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:30 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 10:39:04 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirect(const char *str)
{
	if (str == NULL)
		return (0);
	if (ft_strncmp(str, "<<", 3) == 0
		|| ft_strncmp(str, "<", 2) == 0
		|| ft_strncmp(str, ">>", 3) == 0
		|| ft_strncmp(str, ">", 2) == 0)
		return (1);
	return (0);
}

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
    else if (token[0] == '&')
        return (1);
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
