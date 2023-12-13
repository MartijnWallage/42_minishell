/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/13 16:17:15 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static int	wildcard_match(const char *pattern, const char *name)
{
	while (*pattern == *name || *pattern == '*')
	{
		if (*pattern == 0)
			return (0);
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			while (*name && *name != *pattern)
				name++;
			if (*name == 0 && *pattern != 0)
				return (1);
		}
		else
		{
			pattern++;
			name++;
		}
	}
	return (1);
}

void	expand_wildcards(t_group *group, int index)
{
	DIR				*dir;
	struct dirent	*entry;
	int				is_match;

	if (group->cmd[index] == NULL || strchr(group->cmd[index], '*') == NULL)
		return ;
	dir = opendir(".");
	if (dir == NULL)
	{
		error_msg("could not open current working directory");
		return ;
	}
	is_match = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (wildcard_match(group->cmd[index], entry->d_name) == 0)
			is_match = insert_word(group, entry->d_name, index + 1);
		entry = readdir(dir);
	}
	if (is_match)
		remove_word(group->cmd, index);
	closedir(dir);
}
