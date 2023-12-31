/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:30 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/19 16:02:04 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
