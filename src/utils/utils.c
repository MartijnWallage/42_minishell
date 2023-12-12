/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:30 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 14:36:25 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tab_len(char **tab)
{
	int	size;
	
	size = 0;
	while (tab && tab[size])
		size++;
	return (size);
}

char	**copy_tab(char **tab)
{
	int		size;
	int		i;
	char	**ret;

	size = tab_len(tab);
	ret = malloc((sizeof(char *)) * (size + 1));
	if (!ret)
		return (NULL);
	ret[size] = NULL;
	i = -1;
	while (++i < size)
	{
		ret[i] = ft_strdup(tab[i]);
		if (!ret[i])
		{
			free_tab(tab);
			return (NULL);
		}
	}
	return (ret);
}

char	*mini_getenv(char **env, char *key)
{
	int	i;
	int	j;

	if (!env)
		return (NULL);
	if (!key)
		return (env[0]);
	i = -1;
	while (env[++i])
	{
		j = 0;
		while (env[i][j] && env[i][j] == key[j])
			j++;
		if (key[j] == 0 && env[i][j] == '=')
			return (&env[i][j + 1]);
	}
	return (NULL);
}

t_group	*group_last(t_group *group)
{
	t_group	*current;
	
	current = group;
	while (current && current->next)
		current = current->next;
	return (current);
}
