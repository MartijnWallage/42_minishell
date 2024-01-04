/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/24 17:20:45 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

char	*expand_var(t_group *group, char *old_line, int *dollar_sign)
{
	int		keylen;
	int		valuelen;
	char	*value;
	char	*left_side;
	char	*new_line;

	keylen = get_keylen(&old_line[*dollar_sign + 1]);
	value = get_value(group, &old_line[*dollar_sign + 1]);
	valuelen = ft_strlen(value);
	old_line[*dollar_sign] = 0;
	left_side = ft_strjoin(old_line, value);
	if (value != NULL)
		free(value);
	protect_malloc(group, left_side);
	new_line = ft_strjoin(left_side, &old_line[*dollar_sign + keylen + 1]);
	if (left_side != old_line)
		free(old_line);
	if (new_line != left_side)
		free(left_side);
	protect_malloc(group, new_line);
	*dollar_sign += valuelen - 1;
	return (new_line);
}

static void	find_and_expand_vars(t_group *group, int word_index)
{
	int		i;
	char	waiting_for_quote;

	waiting_for_quote = 0;
	i = -1;
	while (group->cmd[word_index][++i])
	{
		if (waiting_for_quote == group->cmd[word_index][i])
			waiting_for_quote = 0;
		else if (!waiting_for_quote
			&& (group->cmd[word_index][i] == '\''
			|| group->cmd[word_index][i] == '\"'))
			waiting_for_quote = group->cmd[word_index][i];
		else if (waiting_for_quote != '\''
			&& group->cmd[word_index][i] == '$'
			&& (isalnum(group->cmd[word_index][i + 1])
			|| group->cmd[word_index][i + 1] == '?'))
			group->cmd[word_index]
				= expand_var(group, group->cmd[word_index], &i);
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
		if (i > 0)
			expand_wildcards(group, i);
		find_and_expand_vars(group, i);
		remove_quotes(group->cmd[i]);
	}
}
