/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:25 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/03 14:52:34 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_input(char *input)
{
	if (*input == '?') // just to test for now
	{
		ft_printf("%s\n", INVALID_INPUT);
		return (0);
	}
	return (1);
}

char	*reader(void)
{
	char	*str;
	
	str = readline(PROMPT);	// prompt should display username, login name : pwd
	if (!str)
		return (NULL);
	add_history(str);
	if (!is_valid_input(str))
	{
		free(str);
		return (NULL);
	}
	return (str);
}
