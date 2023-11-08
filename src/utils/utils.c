/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:30 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/08 17:41:33 by jmuller          ###   ########.fr       */
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

void	*free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

char	**copy_tab(char **tab)
{
	int		size;
	int		i;
	char	**ret;

	size = tab_len(tab);
	ret = malloc(size * (sizeof(char *) + 1));
	// protect malloc
	ret[size] = NULL;
	i = -1;
	while (++i < size)
		ret[i] = ft_strdup(tab[i]);
	return (ret);
}

int	key_compare(char **env, char *line)
{
	char	*key;
	int		i;

	i = 0;
	key = get_key(line);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0)
			return (1);
		i++;
	}
	return (0);
}

char	*get_key(char *str)
{
	char **kv_pair;
	
	kv_pair = ft_split(str, '=');
	return (kv_pair[0]);
}