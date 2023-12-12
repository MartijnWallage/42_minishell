/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:10:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 20:25:15 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	remove_var(t_group *group, char *key)
{
	int		i;
	int		j;
	int		tablen;
	int		keylen;
	char	**new_env;

	keylen = ft_strlen(key);
	tablen = tab_len(*group->env_ptr);
	new_env = malloc(sizeof(char *) * tablen);
	protect_malloc(group, new_env);
	i = -1;
	j = 0;
	while ((*group->env_ptr)[++i])
	{
		if (ft_strncmp((*group->env_ptr)[i], key, keylen))
		{
			new_env[j] = ft_strdup((*group->env_ptr)[i]);
			protect_malloc(group, new_env[j]);
			j++;
		}
	}
	new_env[j] = NULL;
	free_tab(*group->env_ptr);
	*group->env_ptr = new_env;
}

int	builtin_unset(t_group *group)
{
	int	i;

	i = 0;
	while (group->cmd[++i])
	{
		if (mini_getenv(*group->env_ptr, group->cmd[i]) == NULL)
		{
			*group->exitcode = 0;
			return (1);
		}
		remove_var(group, group->cmd[i]);
	}
	*group->exitcode = 0;
	return (1);
}
