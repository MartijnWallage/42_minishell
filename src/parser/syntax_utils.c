/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 16:23:38 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_left_arg(char **cmd, int index, int *exitcode)
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

int	check_right_arg(char **cmd, int index, int *exitcode)
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
