/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/09 10:11:22 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_first_char(char *str)
{
	if (!str || !*str)
		return ;
	while (str && *str && *(str + 1))
	{
		*str = *(str + 1);
		str++;
	}
	*str = *(str + 1);
}

static void	remove_quotes(char *str)
{
	char	opening_quote;

	while (str && *str)
	{
		if (*str == '\'' || *str == '\"')
		{
			opening_quote = *str;
			remove_first_char(str);
			while (*str && *str != opening_quote)
				str++;
			remove_first_char(str);
		}
		else
			str++; 
	}
}

void	expander(t_group *list)
{
	t_group	*current;
	int		i;

	/* expand variables, except when $ occurs within double quotes 
	Look for $ signs that are not within double quotes
	Then find the variable in env and replace it by its value 
	
	e.g. You can print the current value of PWD by typing echo $PWD in the shell.*/
	current = list;
	while (current)
	{
		i = 0;
		while (current->cmd[i])
		{
			remove_quotes(current->cmd[i]);
			i++;
		}
		current = current->next;
	}
}