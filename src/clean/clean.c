/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:40 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/02 17:00:48 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// history needs be cleared with rl_clear_history()

void	cleanup(t_group *list)
{
	t_group	*temp;

	if (!list)
		return ;
	temp = list->next;
	free_tab(list->cmd);
	free(list);
	cleanup(temp);
}
