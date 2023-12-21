/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:10:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/21 10:59:19 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	unset_var(char **env, char *var)
{
	int	i;
	int	len;

	len = ft_strlen(var);
	i = 0;
	while (env[i] && ft_strncmp(env[i], var, len))
		i++;
	if (env[i])
		remove_word(env, i);
}

int	builtin_unset(t_group *group)
{
	int	i;

	i = 0;
	while (group->cmd[++i])
		unset_var(*group->env_ptr, group->cmd[i]);
	*group->exitcode = 0;
	return (1);
}
