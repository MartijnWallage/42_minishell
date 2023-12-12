/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/11 11:05:08 by mwallage         ###   ########.fr       */
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
    const char		*pattern;
	DIR				*dir;
	struct dirent	*entry;
	int				is_match;
	
	pattern = group->cmd[index];
    if (pattern == NULL || strchr(pattern, '*') == NULL)
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
 		if (wildcard_match(pattern, entry->d_name) == 0)
		{
			is_match = insert_word(group, entry->d_name, index + 1); 
			pattern = group->cmd[index];
		}
		entry = readdir(dir);
	}
	if (is_match)
		remove_word(group->cmd, index);
	closedir(dir);
}
