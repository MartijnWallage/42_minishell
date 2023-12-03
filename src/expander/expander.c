/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/24 17:11:36 by mwallage         ###   ########.fr       */
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

static int	expand_var(t_group *group, int word_index, int dollar_sign)
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
		if (!value)
			return (0);
	}
	else
	{
		while (ft_isalnum(old_word[keylen + dollar_sign + 1]))
			keylen++;
		key = ft_substr(old_word, dollar_sign + 1, keylen);
		if (key == NULL)
			return (0);
		value = mini_getenv(group->env, key);
		if (!value)
			value = ft_strdup("");
		else
			value = ft_strdup(value);
		free(key);
		if (!value)
			return (0);
	}
	old_word[dollar_sign] = 0;
	new_word = ft_strjoin(old_word, value);
	free(value);
	if (!new_word)
		return (0);
	key = new_word;
	new_word = ft_strjoin(new_word, old_word + dollar_sign + keylen + 1);
	free(key);
//	free(old_word);			//	valgrind objects to this, but it seems necessary
	if (!new_word)
		return (0);
	group->cmd[word_index] = new_word;
	return (1);
}

static int	find_and_expand_vars(t_group *group, int word_index)
{
	int		i;
	char	waiting_for_quote;
	char	*word;
	
	word = group->cmd[word_index];
	if (!word)
		return (1);
	waiting_for_quote = 0;
	i = -1;
	while (word[++i])
	{
		if (waiting_for_quote == word[i])
			waiting_for_quote = 0;
		else if (!waiting_for_quote && (word[i] == '\'' || word[i] == '\"'))
			waiting_for_quote = word[i];
		else if (waiting_for_quote != '\'' && word[i] == '$' && isalnum(word[i + 1])
			&& !expand_var(group, word_index, i))
				return (0);
	}
	return (1);
}

int	expander(t_group *list)
{
	t_group	*current;
	int		i;
	
	current = list;
	while (current)
	{
		i = -1;
		while (current->cmd[++i])
		{
			if (!find_and_expand_vars(current, i))
				return (0);
			remove_quotes(current->cmd[i]);
		}
		current = current->next;
	}
	return (1);
}