/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/24 16:39:45 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_first_char(char *str)
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
*/

static void	expand_var(t_group *group, int cmd_index, int dollar_sign)
{
	char	*old_cmd;
	int		keylen;
	char	*new_cmd;
	char	*value;
	char	*key;

	old_cmd = group->cmd[cmd_index];
	keylen = 0;
	if (old_cmd[dollar_sign + 1] == '?')
	{
		keylen = 1;
		value = ft_itoa(group->exitcode);
	}
	else
	{
		while (ft_isalnum(old_cmd[keylen + dollar_sign + 1]))
			keylen++;
		key = ft_substr(old_cmd, dollar_sign + 1, keylen);
		value = mini_getenv(group->env, key);
		free(key);
	}
	old_cmd[dollar_sign] = 0;
	new_cmd = ft_strjoin(old_cmd, value);
	if (old_cmd[dollar_sign + 1] == '?')
		free(value);
	key = new_cmd;
	new_cmd = ft_strjoin(new_cmd, old_cmd + dollar_sign + keylen + 1);
	free(key);
	free(old_cmd);
	group->cmd[cmd_index] = new_cmd;
}

static void	find_and_expand_vars(t_group *group, int index)
{
	int		i;
	char	waiting_for_quote;
	char	*cmd;
	
	cmd = group->cmd[index];
	waiting_for_quote = 0;
	i = -1;
	while (cmd[++i])
	{
		if (waiting_for_quote == cmd[i])
			waiting_for_quote = 0;
		else if (!waiting_for_quote && (cmd[i] == '\'' || cmd[i] == '\"'))
			waiting_for_quote = cmd[i];
		else if (waiting_for_quote != '\'' && cmd[i] == '$' && cmd[i + 1])
			expand_var(group, index, i);
	}
}

void	expander(t_group *list)
{
	t_group	*current;
	int		i;
	
	current = list;
	while (current)
	{
		i = -1;
		while (current->cmd[++i])
		{
			find_and_expand_vars(current, i);
			remove_quotes(current->cmd[i]);
		}
		current = current->next;
	}
}