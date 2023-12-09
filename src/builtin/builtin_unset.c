/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:10:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/05 10:58:08 by mwallage         ###   ########.fr       */
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
	tablen = tab_len(*group->mini_env);
	new_env = malloc(sizeof(char *) * tablen);
	protect_malloc(group, new_env);
	i = -1;
	j = 0;
	while ((*group->mini_env)[++i])
	{
		if (ft_strncmp((*group->mini_env)[i], key, keylen))
		{
			new_env[j] = ft_strdup((*group->mini_env)[i]);
			protect_malloc(group, new_env[j]);
			j++;
		}
	}
	new_env[j] = NULL;
	free_tab(*group->mini_env);
	*group->mini_env = new_env;
}

int	builtin_unset(t_group *group)
{
	int	i;

	i = 0;
	while (group->cmd[++i])
	{
		if (mini_getenv(*group->mini_env, group->cmd[i]) == NULL)
		{
			*group->exitcode = 0;
			return (1);
		}
		remove_var(group, group->cmd[i]);
	}
	*group->exitcode = 0;
	return (1);
}