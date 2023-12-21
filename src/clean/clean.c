/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:40 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/21 16:45:28 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	cleanup(head->subshell);
	if (head->original_stdin != STDIN_FILENO)
		close(head->original_stdin);
	if (head->infile != STDIN_FILENO)
		close(head->infile);
	if (head->heredoc != STDIN_FILENO)
		close(head->heredoc);
	if (head->original_stdout != STDOUT_FILENO)
		close(head->original_stdout);
	if (head->outfile != STDOUT_FILENO)
		close(head->outfile);
	temp = head->next;
	free(head);
	if (temp)
		temp->previous = NULL;
	cleanup(temp);
}

void	cleanup_and_exit(t_group *list, int exitcode)
{
	rl_clear_history();
	free_tab(*list->env_ptr);
	list->env_ptr = NULL;
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

void	protect_malloc_during_build(char **cmd, char ***env_ptr, void *ptr)
{
	if (ptr == NULL)
	{
		free_tab(cmd);
		free_tab(*env_ptr);
		error_msg(MALLOC_MSG);
		rl_clear_history();
		exit(MALLOC_CODE);
	}
}

void	*protect_and_free(t_group *group, void *ptr, char **tab, char *str)
{
	if (ptr == NULL)
	{
		free_tab(tab);
		if (str)
			free(str);
		error_msg(MALLOC_MSG);
		cleanup_and_exit(group, MALLOC_CODE);
	}
	return (NULL);
}
