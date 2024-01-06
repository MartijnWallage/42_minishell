/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tab.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:30 by mwallage          #+#    #+#             */
/*   Updated: 2024/01/06 21:26:56 by mwallage         ###   ########.fr       */
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

void	*free_tab(char **tab)
{
	int	i;

	if (!tab)
		return (NULL);
	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
	return (NULL);
}

void	remove_word(char **tab, int index)
{
	int	i;

	free(tab[index]);
	i = index;
	while (tab[i + 1])
	{
		tab[i] = tab[i + 1];
		i++;
	}
	tab[i] = NULL;
}
