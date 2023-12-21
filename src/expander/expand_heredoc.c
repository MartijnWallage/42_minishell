/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/21 10:12:31 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static char	*expand_heredoc_var(t_group *group, char *old_line, int dollar_sign)
{
	int		keylen;
	char	*value;
	char	*left_side;
	char	*new_line;

	keylen = get_keylen(&old_line[dollar_sign + 1]);
	value = get_value(group, &old_line[dollar_sign + 1]);
	old_line[dollar_sign] = 0;
	left_side = ft_strjoin(old_line, value);
	if (value != NULL)
		free(value);
	new_line = ft_strjoin(left_side, &old_line[dollar_sign + keylen + 1]);
	if (left_side != old_line)
		free(old_line);
	if (new_line != left_side)
		free(left_side);
	return (new_line);
}

static char	*find_and_expand_heredoc_vars(t_group *group, char *line)
{
	int		i;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '$' && (isalnum(line[i + 1]) || line[i + 1] == '?'))
			line = expand_heredoc_var(group, line, i);
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
