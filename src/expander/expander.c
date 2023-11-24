/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/24 17:09:03 by mwallage         ###   ########.fr       */
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

static void	expand_var(t_group *group, int word_index, int dollar_sign)
{
	char	*old_word;
	int		keylen;
	char	*new_word;
	char	*value;
	char	*key;

	old_word = group->cmd[word_index];
	keylen = 0;
	if (old_word[dollar_sign + 1] == '?')
	{
		keylen = 1;
		value = ft_itoa(group->exitcode);
	}
	else
	{
		while (ft_isalnum(old_word[keylen + dollar_sign + 1]))
			keylen++;
		key = ft_substr(old_word, dollar_sign + 1, keylen);
		value = mini_getenv(group->env, key);
		free(key);
	}
	old_word[dollar_sign] = 0;
	new_word = ft_strjoin_safe(old_word, value);
	if (old_word[dollar_sign + 1] == '?')
		free(value);
	key = new_word;
	new_word = ft_strjoin(new_word, old_word + dollar_sign + keylen + 1);
	free(key);
	free(old_word);
	group->cmd[word_index] = new_word;
}

static void	find_and_expand_vars(t_group *group, int word_index)
{
	int		i;
	char	waiting_for_quote;
	char	*word;
	
	word = group->cmd[word_index];
	waiting_for_quote = 0;
	i = -1;
	while (word[++i])
	{
		if (waiting_for_quote == word[i])
			waiting_for_quote = 0;
		else if (!waiting_for_quote && (word[i] == '\'' || word[i] == '\"'))
			waiting_for_quote = word[i];
		else if (waiting_for_quote != '\'' && word[i] == '$' && word[i + 1])
			expand_var(group, word_index, i);
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