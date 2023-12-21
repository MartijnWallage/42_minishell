/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/21 18:17:40 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static char	*find_and_expand_heredoc_vars(t_group *group, char *line)
{
	int		i;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '$' && (isalnum(line[i + 1]) || line[i + 1] == '?'))
			line = expand_var(group, line, &i);
	}
	return (line);
}

void	expand_heredoc(t_group *group)
{
	int		i;
	int		pipefd[2];
	char	*line;

	i = tab_len(group->cmd);
	while (--i >= 0)
	{
		if (group->cmd[i][0] == '<' && group->cmd[i][1] == '<'
			&& (!is_quotation_mark(group->cmd[i + 1][0])))
		{
			if (pipe(pipefd) == -1)
			{
				error_msg("heredoc expansion pipe failed");
				return ;
			}
			while (1)
			{
				line = get_next_line(group->heredoc);
				if (line == NULL)
					break ;
				line = find_and_expand_heredoc_vars(group, line);
				ft_putstr_fd(line, pipefd[1]);
				free(line);
			}
			close(pipefd[1]);
			close(group->heredoc);
			group->heredoc = pipefd[0];
			return ;
		}
	}
}
