/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:40 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/03 14:06:57 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup(t_group *list)
{
	t_group	*temp;

	if (!list)
		return ;
	temp = list->next;
	free_tab(list->cmd);
	free(list);
	cleanup(temp);
//	rl_clear_history();		// Somehow this makes the history not work. What to do?
}
