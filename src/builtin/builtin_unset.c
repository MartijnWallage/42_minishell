/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:10:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/08 17:56:08 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Syntax
	unset var1 var2 ...

	
*/

void	remove_env(t_group *group, char *line)
{
	int		i;
	int		j;
	int		row;
	char	*key;
	char	**env2;

	i = 0;
	j = 0;
	row = tab_len(group->env);
	env2 = malloc(sizeof(char *) * row);
	// protect malloc
	key = get_key(line);
	while (j < row)
	{
		if (!(ft_strncmp(group->env[j], key, ft_strlen(key))))
			j++;
		else
		{
			env2[i] = ft_strdup(group->env[j]);
			i++;
			j++;
		}
	}
	env2[i] = NULL;
	free_tab(group->env);
	group->env = env2;
}

void	builtin_unset(t_group *group)
{
	int	j;

	j = 1;
	while (group->cmd[j])
	{	
		if (key_compare(group->env, group->cmd[j]) == 0)
			return ;
		remove_env(group, group->cmd[j]);
		j++;
	}
}