/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:04 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/01 15:44:18 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*init_token(char *value, int type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	return (token);
}

char	*double_quote(char *line)
{

}

char	*single_quote(char *line)
{
	// special case: /"
}

char	**lexer(char *line)
{
	char	**tab;

	/*	It would be great to return an array of tokens,
	 *	instead of an array of strings
	 *	But it makes execution a little more complicated, 
	 *	since execve() wants an array of strings 			*/
	tab = token_split(line);
	if (!tab)
		handle_error(MALLOC_MSG, MALLOC_CODE);
	// take care of quotes etc.
	return (tab);
}
