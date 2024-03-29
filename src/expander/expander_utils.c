/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2024/01/06 21:26:47 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

int	get_keylen(char *word)
{
	int	keylen;

	if (word[0] == '?')
		return (1);
	keylen = 0;
	while (ft_isalnum(word[keylen]))
		keylen++;
	return (keylen);
}

char	*get_value(t_group *group, char *word)
{
	int		keylen;
	char	*value;
	char	*key;

	if (word[0] == '?')
	{
		value = ft_itoa(*group->exitcode);
		protect_malloc(group, value);
		return (value);
	}
	keylen = get_keylen(word);
	key = ft_substr(word, 0, keylen);
	protect_malloc(group, key);
	value = mini_getenv(*group->env_ptr, key);
	free(key);
	if (value)
	{
		value = ft_strtrim(value, " ");
		protect_malloc(group, value);
	}
	return (value);
}

static int	copy_left_side(t_group *group, char **new_cmd, char **old_cmd, \
	int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		new_cmd[i] = ft_strdup(old_cmd[i]);
		if (new_cmd[i] == NULL)
		{
			free_tab(new_cmd);
			protect_malloc(group, NULL);
		}
		i++;
	}
	return (i);
}

int	insert_word(t_group *group, char *new_word, int index)
{
	int		i;
	int		tablen;
	char	**new_cmd;

	tablen = tab_len(group->cmd);
	new_cmd = malloc(sizeof(char *) * (tablen + 2));
	protect_malloc(group, new_cmd);
	i = copy_left_side(group, new_cmd, group->cmd, index);
	new_cmd[i] = ft_strdup(new_word);
	protect_malloc(group, new_cmd[i]);
	while (++i < tablen + 1)
	{
		new_cmd[i] = ft_strdup(group->cmd[i - 1]);
		if (new_cmd[i] == NULL)
		{
			free_tab(new_cmd);
			protect_malloc(group, NULL);
		}
	}
	new_cmd[i] = NULL;
	free_tab(group->cmd);
	group->cmd = new_cmd;
	return (1);
}
