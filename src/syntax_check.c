/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 11:43:10 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int is_matching_quotes(const char *str)
{
	char	quote;

	if (*str == 0)
		return (1);
	while (*str && !is_quotation_mark(*str))
		str++;
	if (*str == 0)
		return (1);
	quote = *str;
	while (*++str && *str != quote)
		;
	if (*str == 0)
		return (0);
	return (is_matching_quotes(++str));
}

int	is_valid_redirect(char *filename)
{
	if (filename == NULL || is_meta_char(filename[0]))
		return (0);
	return (1);
}

int	is_valid_syntax(char **cmd, int *exitcode)
{
	int	i;
	int	counter;

	if (cmd == NULL)
		return (0);
	counter = 1;
	i = -1;
	while (cmd[++i] && counter)
	{
		counter += cmd[i][0] == '(';
		counter -= cmd[i][0] == ')';
	}
	if (counter != 1)
		return (syntax_error("()", exitcode), 0);
	i = -1;
	while (cmd[++i])
	{
		if (!is_matching_quotes(cmd[i]))
			return (i);
		if (cmd[i][0] == '(' && cmd[i + 1][0] == ')')
			return (syntax_error("()", exitcode), 0);

		/* if (is_control_operator(cmd[i]) && cmd[i] != '(' && cmd[i] != ')')
		{
			// find argument to the left and to the right
			// check if argument is valid
		} */
		if (is_redirect(cmd[i]) && !is_valid_redirect(cmd[i + 1]))
			return (syntax_error(cmd[i + 1], exitcode), 0);
	}
	return (1);
}