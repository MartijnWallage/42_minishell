/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:40 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/07 19:41:11 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_tab(char **tab)
{
	int	i;

	if (!tab)
		return (NULL);
	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab[i]);
	free(tab);
	return (NULL);
}

void	cleanup(t_group *list)
{
	t_group	*temp;
	t_group	*head;

	if (!list)
		return ;
	head = list;
	while (head && head->previous)
		head = head->previous;
	free_tab(head->cmd);
	temp = head->next;
	free(head);
	if (temp)
	{
		temp->previous = NULL;
		cleanup(temp);
	}
}

void	cleanup_and_exit(t_group *list, int exitcode)
{
	rl_clear_history();
	free_tab(*list->mini_env);
	list->mini_env = NULL;
	cleanup(list);
	exit(exitcode);
}

void	protect_malloc(t_group *group, void *ptr)
{
	if (ptr == NULL)
	{
		error_msg(MALLOC_MSG);
		cleanup_and_exit(group, MALLOC_CODE);
	}
}

void	protect_malloc_during_build(char **cmd, char ***mini_env, void *ptr)
{
	if (ptr == NULL)
	{
		free_tab(cmd);
		free_tab(*mini_env);
		error_msg(MALLOC_MSG);
		rl_clear_history();
		exit(MALLOC_CODE);
	}
}