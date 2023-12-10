/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 17:28:04 by mwallage         ###   ########.fr       */
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
		value = ft_strdup(value);
		protect_malloc(group, value);
	}
	return (value);
}

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

void	remove_word(char **tab, int index)
{
	int	i;

	i = index;
	while (tab[i + 1])
	{
		tab[i] = tab[i + 1];
		i++;
	}
	free(tab[i + 1]);
	tab[i] = NULL;
}

int	insert_word(t_group *group, char *new_word, int index)
{
	int		i;
	int		tablen;
	char	**new_cmd;

	tablen = tab_len(group->cmd);
	new_cmd = malloc(sizeof(char *) * (tablen + 2));
	protect_malloc(group, new_cmd);
	i = 0;
	while (i < index)
	{
		new_cmd[i] = ft_strdup(group->cmd[i]);
		protect_malloc(group, new_cmd[i]);
		i++;
	}
	new_cmd[i] = ft_strdup(new_word);
	protect_malloc(group, new_cmd[i]);
	while (++i < tablen + 1)
	{
		new_cmd[i] = ft_strdup(group->cmd[i - 1]);
		protect_malloc(group, new_cmd[i]);	// there's a leak here. new_cmd is not getting freed. Also in export
	}
	new_cmd[i] = NULL;
	free_tab(group->cmd);
	group->cmd = new_cmd;
	return (1);
}
