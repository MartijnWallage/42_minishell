/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/13 16:21:05 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static void	join_right_side(t_group *group, int i, int dollar, int keylen)
{
	char	*temp;
	char	*word;

	word = group->cmd[i];
	temp = group->cmd[i];
	group->cmd[i] = ft_strjoin(group->cmd[i], &word[dollar + keylen + 1]);
	if (word != temp)
		free(word);
	if (temp != group->cmd[i])
		free(temp);
	protect_malloc(group, group->cmd[i]);
}

static char	*expand_var(t_group *group, int word_index, int *dollar_sign)
{
	int		keylen;
	int		valuelen;
	char	*word;
	char	*value;

	word = group->cmd[word_index];
	keylen = get_keylen(&word[*dollar_sign + 1]);
	value = get_value(group, &word[*dollar_sign + 1]);
	word[*dollar_sign] = 0;
	group->cmd[word_index] = ft_strjoin(word, value);
	valuelen = ft_strlen(value);
	if (value)
		free(value);
	protect_malloc(group, group->cmd[word_index]);
	if (word[*dollar_sign + keylen + 1])
		join_right_side(group, word_index, *dollar_sign, keylen);
	*dollar_sign += valuelen - 1;
	return (group->cmd[word_index]);
}

static void	find_and_expand_vars(t_group *group, int word_index)
{
	int		i;
	char	waiting_for_quote;
	char	*word;

	word = group->cmd[word_index];
	waiting_for_quote = 0;
	i = -1;
	while (word[++i])
	{
		if (waiting_for_quote == word[i])
			waiting_for_quote = 0;
		else if (!waiting_for_quote && (word[i] == '\'' || word[i] == '\"'))
			waiting_for_quote = word[i];
		else if (waiting_for_quote != '\'' && word[i] == '$'
			&& (isalnum(word[i + 1]) || word[i + 1] == '?'))
			word = expand_var(group, word_index, &i);
	}
}

char	*expand_heredoc_var(t_group *group, char *old_line, int dollar_sign)
{
 	int		keylen;
	char	*value;
	char	*new_line;
	char	*temp;

 	keylen = get_keylen(&old_line[dollar_sign + 1]);
	value = get_value(group, &old_line[dollar_sign + 1]);
	old_line[dollar_sign] = 0;
	new_line = ft_strjoin(old_line, value);
	temp = new_line;
	new_line = ft_strjoin(new_line, &old_line[dollar_sign + keylen + 1]);
	free(temp);
	free(old_line);
	return (new_line);
}

char	*find_and_expand_heredoc_vars(t_group *group, char *line)
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
			}
			close(pipefd[1]);
			close(group->heredoc);
			group->heredoc = pipefd[0];
			return ;
		}
	}
}

void	expander(t_group *group)
{
	int		i;

	if (!group || !group->cmd)
		return ;
	remove_redirect(group->cmd);
	i = -1;
	while (group->cmd[++i])
	{
		expand_wildcards(group, i);
		find_and_expand_vars(group, i);
		remove_quotes(group->cmd[i]);
	}
}
