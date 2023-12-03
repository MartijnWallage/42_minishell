/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/03 18:57:48 by mwallage         ###   ########.fr       */
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

static int	get_keylen(char *word)
{
	int	keylen;

	keylen = 0;
	while (ft_isalnum(word[keylen]))
		keylen++;
	return (keylen);
}

static char	*expand_var(t_group *group, char *word)
{
	int		keylen;
	char	*value;
	char	*key;

	if (word[0] == '?')
	{
		value = ft_itoa(group->exitcode);
		protect_malloc(group, value);
		return (value);
	}
	keylen = get_keylen(word);
	key = ft_substr(word, 0, keylen);
	protect_malloc(group, key);
	value = ft_strdup(mini_getenv(group->env, key));
	protect_malloc(group, value);
	return (value);
}

static void	find_and_expand_vars(t_group *group, int word_index)
{
	int		i;
	char	waiting_for_quote;
	char	*word;
	char	*value;
	char	*temp;
	
	word = group->cmd[word_index];
	waiting_for_quote = 0;
	i = -1;
	while (word[++i])
	{
		if (waiting_for_quote == word[i])
			waiting_for_quote = 0;
		else if (!waiting_for_quote && (word[i] == '\'' || word[i] == '\"'))
			waiting_for_quote = word[i];
		else if (waiting_for_quote != '\'' && word[i] == '$' && isalnum(word[i + 1]))
		{
			value = expand_var(group, &word[i + 1]);
			word[i] = 0;
			group->cmd[word_index] = ft_strjoin(word, value);
			protect_malloc(group, group->cmd[word_index]);
			temp = group->cmd[word_index];
			group->cmd[word_index] = ft_strjoin(group->cmd[word_index], word + ft_strlen(value));
			protect_malloc(group, group->cmd[word_index]);
			i += ft_strlen(value);
			free(temp);
			free(word);
			free(value);
		}
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