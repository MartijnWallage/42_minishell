/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/20 17:18:44 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static void	join_right_side(t_group *group, int i, char *right_side)
{
	char	*word;

	word = ft_strjoin(group->cmd[i], right_side);
	protect_malloc(group, word);
	free(group->cmd[i]);
	group->cmd[i] = word;
}

static char	*expand_var(t_group *group, int word_index, int *dollar_sign)
{
	int		keylen;
	int		valuelen;
	char	*word;
	char	*value;
	char	*right_side;

	word = group->cmd[word_index];
	keylen = get_keylen(&word[*dollar_sign + 1]);
	value = get_value(group, &word[*dollar_sign + 1]);
	valuelen = ft_strlen(value);
	word[*dollar_sign] = 0;
	group->cmd[word_index] = ft_strjoin(word, value);
	if (value)
		free(value);
	protect_malloc(group, group->cmd[word_index]);
	right_side = &word[*dollar_sign + keylen + 1];
	if (right_side)
		join_right_side(group, word_index, right_side);
	free(word);
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
