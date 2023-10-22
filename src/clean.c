/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:40 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/22 18:14:43 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_list(t_group *list)
{
	t_group	*temp;

	if (!list)
		return ;
	temp = list->next;
	free_tab(list->cmd);
	free(list);
	free_list(temp);
}

void	free_vars(t_var *vars)
{
	free(vars->line);
	free(vars);
}

void	cleanup(t_var *vars, t_group *list)
{
	free_list(list);
	free_vars(vars);
}
