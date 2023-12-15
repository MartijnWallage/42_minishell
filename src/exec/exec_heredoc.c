/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 15:20:23 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/14 18:32:11 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	close_previous_infile(t_group *group)
{
	if (group->infile)
	{
		close(group->infile);
		ft_dup2(group, group->original_stdin, STDIN_FILENO);
		group->infile = group->original_stdin;
	}
}

int	open_heredoc(t_group *group)
{
	close_previous_infile(group);
	if (ft_dup2(group, group->heredoc, STDIN_FILENO) == -1)
		return (close(group->heredoc), 0);
	close(group->heredoc);
	return (1);
}
