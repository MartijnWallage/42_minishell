/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:10:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/20 16:43:08 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Syntax
	unset var1 var2 ...
	
*/

void	remove_env(t_group *group, char *key)
{
	int		i;
	int		j;
	int		tablen;
	int		keylen;
	char	**new_env;

	keylen = ft_strlen(key);
	tablen = tab_len(group->env);
	new_env = malloc(sizeof(char *) * tablen);
	// protect malloc
	i = -1;
	j = 0;
	while (group->env[++i])
	{
		if (ft_strncmp(group->env[i], key, keylen))
		{
			new_env[j] = ft_strdup(group->env[i]);
			j++;
		}
	}
	new_env[j] = NULL;
	free_tab(group->env);
	group->env = new_env;
}

void	builtin_unset(t_group *group)
{
	int	i;

	i = 1;
	while (group->cmd[i])
	{
		if (mini_getenv(group->env, group->cmd[i]) == NULL)
			return ;
		remove_env(group, group->cmd[i]);
		i++;
	}
}