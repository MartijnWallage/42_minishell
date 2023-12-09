/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 15:21:47 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	remove_word(char **tab, int index)
{
	int	i;

	i = index;
	while (tab[i + 1])
	{
		tab[i] = tab[i + 1];
		i++;
	}
	free(tab[i + 1]);
	tab[i] = NULL;
}

void	remove_redirect(char **cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (cmd[i])
	{
		if (is_redirect(cmd[i]))
		{
			remove_word(cmd, i + 1);
			remove_word(cmd, i);
		}
		else
			i++;
	}
}
