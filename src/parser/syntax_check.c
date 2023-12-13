/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 16:23:38 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	is_matching_quotes(const char *str)
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

static int	check_left_arg(char **cmd, int index, int *exitcode)
{
	int	j;

	j = index - 1;
	while (j >= 0 && (cmd[j][0] == ')' || cmd[j][0] == '('))
		j--;
	if (j < 0)
		return (syntax_error(cmd[index], exitcode), 0);
	if (is_control_operator(cmd[j]))
		return (syntax_error(cmd[j], exitcode), 0);
	return (1);
}

static int	check_right_arg(char **cmd, int index, int *exitcode)
{
	int	j;

	j = index + 1;
	while (cmd[j] && (cmd[j][0] == '(' || cmd[j][0] == ')'))
		j++;
	if (cmd[j] == NULL)
		return (syntax_error(cmd[index], exitcode), 0);
	if (is_control_operator(cmd[j]))
		return (syntax_error(cmd[j], exitcode), 0);
	return (1);
}

static int	check_parentheses(char **cmd, int *exitcode)
{
	int	counter;
	int	i;

	counter = 1;
	i = -1;
	while (cmd[++i] && counter)
	{
		counter += cmd[i][0] == '(';
		counter -= cmd[i][0] == ')';
		if (cmd[i][0] == '(' && cmd[i + 1] && cmd[i + 1][0] == ')')
			return (syntax_error("empty parentheses", exitcode), 0);
	}
	if (counter != 1)
		return (syntax_error("missing parenthesis", exitcode), 0);
	return (1);
}

int	check_redirect(char **cmd, int index, int *exitcode)
{
	if (!is_redirect(cmd[index]))
		return (1);
	if (cmd[index + 1] == NULL || is_meta_char(cmd[index + 1][0]))
	{
		syntax_error(cmd[index + 1], exitcode);
		return (0);
	}
	return (1);
}

int	is_valid_syntax(char **cmd, int *exitcode)
{
	int	i;

	if (cmd == NULL)
		return (0);
	if (!check_parentheses(cmd, exitcode))
		return (0);
	i = -1;
	while (cmd[++i])
	{
		if (!is_matching_quotes(cmd[i]))
			return (syntax_error(cmd[i], exitcode), 0);
		if (is_control_operator(cmd[i]) && cmd[i][0] != '(' && cmd[i][0] != ')')
		{
			if (!check_left_arg(cmd, i, exitcode))
				return (0);
			if (!check_right_arg(cmd, i, exitcode))
				return (0);
		}
		if (cmd[i][0] == '(' && i > 0 && !is_control_operator(cmd[i - 1]))
			return (syntax_error(cmd[i], exitcode), 0);
		if (cmd[i][0] == ')' && cmd[i + 1] && !is_control_operator(cmd[i + 1]))
			return (syntax_error(cmd[i], exitcode), 0);
		if (!check_redirect(cmd, i, exitcode))
			return (0);
	}
	return (1);
}
