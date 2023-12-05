/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/05 10:09:57 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static char	*expand_var(t_group *group, int word_index, int *dollar_sign)
{
	int		keylen;
	int		valuelen;
	char	*word;
	char	*value;
	char	*temp;

	word = group->cmd[word_index];
	keylen = get_keylen(&word[*dollar_sign + 1]);
	value = get_value(group, &word[*dollar_sign + 1]);
	word[*dollar_sign] = 0;
	group->cmd[word_index] = ft_strjoin(word, value);
	valuelen = ft_strlen(value);
	if (value)
		free(value);
	protect_malloc(group, group->cmd[word_index]);
	if (word[*dollar_sign + keylen + 1])
	{
		temp = group->cmd[word_index];
		group->cmd[word_index] =
			ft_strjoin(group->cmd[word_index], &word[*dollar_sign + keylen + 1]);
		if (word != temp)
			free(word);
		if (temp != group->cmd[word_index])
			free(temp);
		protect_malloc(group, group->cmd[word_index]);		
	}
	*dollar_sign += valuelen - 1;
	return (group->cmd[word_index]);
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
		if (waiting_for_quote ==  word[i])
			waiting_for_quote = 0;
		else if (!waiting_for_quote && (word[i] == '\'' || word[i] == '\"'))
			waiting_for_quote = word[i];
		else if (waiting_for_quote != '\'' && word[i] == '$'
			&& (isalnum(word[i + 1]) || word[i + 1] == '?'))
			word = expand_var(group, word_index, &i);
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