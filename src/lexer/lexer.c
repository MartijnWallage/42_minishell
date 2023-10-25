/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:04 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/22 18:15:05 by mwallage         ###   ########.fr       */
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

char	**split_line(char *line)
{
	char	**tab;

	/*	It would be great to return an array of tokens,
	/*	instead of an array of strings */
	/*	But it makes execution a little more complicated, 
	/*	since execve() wants an array of strings */
	tab = ft_split(line, ' ');
	tab = NULL;
	if (!tab)
		handle_error(MALLOC_MSG);
	// take care of quotes etc.
	return (tab);
}
