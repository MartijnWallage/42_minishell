/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/21 17:21:48 by mwallage         ###   ########.fr       */
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

	keylen = get_keylen(&group->cmd[word_index][*dollar_sign + 1]);
	value = get_value(group, &group->cmd[word_index][*dollar_sign + 1]);
	valuelen = ft_strlen(value);
	group->cmd[word_index][*dollar_sign] = 0;
	word = ft_strjoin(group->cmd[word_index], value);
	if (value)
	{
		free(value);
		protect_malloc(group, word);
	}
	if (group->cmd[word_index][*dollar_sign + keylen + 1] != '\0')
	{
		temp = word;
		word = ft_strjoin(word, group->cmd[word_index] + *dollar_sign + keylen + 1);
		if (temp != group->cmd[word_index])
			free(temp);
		protect_malloc(group, word);
	}
	if (word != group->cmd[word_index])
		free(group->cmd[word_index]);
	group->cmd[word_index] = word;
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
		if (waiting_for_quote == word[i])
			waiting_for_quote = 0;
		else if (!waiting_for_quote && (word[i] == '\'' || word[i] == '\"'))
			waiting_for_quote = word[i];
		else if (waiting_for_quote != '\'' && word[i] == '$'
			&& (isalnum(word[i + 1]) || word[i + 1] == '?'))
			word = expand_var(group, word_index, &i);
	}
}

void	expander(t_group *group)
{
	int		i;

	if (!group || !group->cmd)
		return ;
	remove_redirect(group->cmd);
	i = -1;
	while (group->cmd[++i])
	{
		expand_wildcards(group, i);
		find_and_expand_vars(group, i);
		remove_quotes(group->cmd[i]);
	}
}
