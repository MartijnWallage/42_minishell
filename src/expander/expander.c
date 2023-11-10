/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/10 14:31:53 by jmuller          ###   ########.fr       */
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

/*
	Features:
	- expands variables, e.g. $PATH
	- expands variables in variables
	  - e.g. export var=$PATH

	To do:
	- $?: should expand to the exit status of the most recently executed
	foreground pipeline.
	- expandable variables in strings?
*/

static void	expand_variables(char **str, t_group *group)
{
	int	i;
	char *value;
	
	if (*str[0] == '$')
	{
		i = 0;
		while (group->env[i])
		{
			if (ft_strncmp(group->env[i], *str + 1, ft_strlen(*str + 1)) == 0)
			{
				value = get_value(group->env[i]);
				*str = value;
				if (*str[0] == '$')
					expand_variables(str, group);
			}
			i++;
		}
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
			expand_variables(&current->cmd[i], current);
			i++;
		}
		current = current->next;
	}
}