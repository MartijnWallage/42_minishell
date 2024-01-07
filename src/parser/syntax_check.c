/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2024/01/07 13:10:55 by mwallage         ###   ########.fr       */
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
		{
			syntax_error("empty parentheses", exitcode);
			return (0);
		}
	}
	if (counter != 1)
	{
		syntax_error("missing parenthesis", exitcode);
		return (0);
	}
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

/**
 * @brief Checks 2D-Array produced by tokenizer for syntax errors
 * in command strings
 *
 * This function 
 * 1. Verifies that parentheses are correctly matched.
 * 2. Ensures that quotes within each command string are properly closed.
 * 3. Checks for correct syntax around control operators (eg. logical operators).
 * 4. Validates the syntax for command redirections.
 * @param cmd Double pointer to 2D-Array of strings produced by tokenizer.
 * @param exitcode Pointer to an integer where the exitcode can be stored.
 * @return Returns 0 if it finds an error and 1 if no error is detected
 */

int	is_valid_syntax(char **cmd, int *exitcode)
{
	int	i;

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
