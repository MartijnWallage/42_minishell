/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/16 15:08:52 by jmuller          ###   ########.fr       */
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

static int	remove_quotes(char *str)
{
	char	opening_quote;
	int		flag;

	flag = 1;
	while (str && *str)
	{
		if (*str == '\'' || *str == '\"')
		{
			if (*str == '\'')
				flag = 0;
			opening_quote = *str;
			remove_first_char(str);
			while (*str && *str != opening_quote)
				str++;
			remove_first_char(str);
		}
		else
			str++; 
	}
	return (flag);
}

/*
	Features:
	- expands variables, e.g. $PATH
	- expands variable ins strings echo "$var1 text $var2 text"
	- expands variables in variables
	  - e.g. export VAR=$PATH
	  

	To do:
	- $?: should expand to the exit status of the most recently executed
	foreground pipeline.

	Errors:
	- Segfault: "export VAR1= Hans" 
	- Easy to solve with value_check fct. Need to know error code.
*/

static void	expand_variables(char **str, t_group *group)
{
	int		i;
	char	*value;
	
	i = 0;
	while (group->env[i])
	{
		if ((ft_strncmp(group->env[i], *str + 1, ft_strlen(*str + 1)) == 0) \
			&& ft_strlen(*str + 1) != 0)
		{
			value = get_value(group->env[i]);
			free(*str);
			*str = value;
			if (*str[0] == '$')
				expand_variables(str, group);
		}
		i++;
	}
}

static void	expand_string(char **str, t_group *group)
{
	char	**temp;
	char	*joined;
	char	*helper;
	int		i;
	
	joined = NULL;
	i = 0;
	temp = ft_split(*str, ' ');
	while (temp[i])
	{
		if (temp[i][0] == '$')
			expand_variables(&temp[i], group);
		i++;
	}
	i = 0;
	while (temp[i])
	{
		helper = ft_strjoin_safe(joined, temp[i]);
		if (!(joined == NULL))
			free(joined);
		joined = helper;
		if (temp[i + 1])
		{
			helper = ft_strjoin(joined, " ");
			free(joined);
			joined = helper;
		}
		i++;
	}
	free_tab(temp);
	free(*str);
	*str = joined;	
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
			//printf("String[%d]: %s\n", i, current->cmd[i]);
			if (remove_quotes(current->cmd[i]))
				expand_string(&current->cmd[i], current);
			i++;
		}
		current = current->next;
	}
}