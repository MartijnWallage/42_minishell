/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/23 16:24:14 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_word(char **tab, int index)
{
	int	i;

	i = index;
	while (tab[i + 1])
	{
		tab[i] = tab[i + 1];
		i++;
	}
	tab[i] = NULL;
}

void	here_doc(t_group *group, int index)
{
	if (group->cmd[index] == NULL)
	{
		remove_word(group->cmd, index - 1);
		err_and_exit("syntax error near unexpected token 'newline'", 2);
		return ;
	}
	group->heredoc_delimiter = group->cmd[index];
	if (group->infile != STDIN_FILENO)
		close(group->infile);
	group->infile = STDIN_FILENO;
	remove_word(group->cmd, index);
	remove_word(group->cmd, index - 1);
}

void	redirect_in(t_group *group, int index)
{
	if (group->cmd[index] == NULL)
	{
		remove_word(group->cmd, index - 1);
		group->exitcode = 2;
		err_and_exit("syntax error near unexpected token 'newline'", 2);
		return ;
	}
	if (group->infile != STDIN_FILENO)
		close(group->infile);
	group->infile = open(group->cmd[index], O_RDONLY, 0777);
	if (group->infile == -1)
		group->infile = STDIN_FILENO;
	remove_word(group->cmd, index);
	remove_word(group->cmd, index - 1);
}

void	redirect_out(t_group *group, int index, bool append)
{
	if (group->cmd[index] == NULL)
	{
		remove_word(group->cmd, index - 1);
		err_and_exit("syntax error near unexpected token 'newline'", 2);
		return ;
	}
	if (group->outfile != STDOUT_FILENO)
		close(group->outfile);
	if (append)
		group->outfile = open(group->cmd[index], O_WRONLY | O_CREAT | O_APPEND, 0777);
	else
		group->outfile = open(group->cmd[index], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (group->outfile == -1)
		group->outfile = STDOUT_FILENO;
	remove_word(group->cmd, index);
	remove_word(group->cmd, index - 1);
}

void	parse_redirect(t_group *group)
{
	int	i;

	i = 0;
	while (group->cmd[i] && group->cmd[i][0] != '|')
	{
		if (group->cmd[i][0] == '<' && group->cmd[i][1] == '<')
			here_doc(group, i + 1);
		else if (group->cmd[i][0] == '<')
			redirect_in(group, i + 1);
		else if (group->cmd[i][0] == '>' && group->cmd[i][1] == '>')
			redirect_out(group, i + 1, true);
		else if (group->cmd[i][0] == '>')
			redirect_out(group, i + 1, false);
		else
			i++;
	}
}
