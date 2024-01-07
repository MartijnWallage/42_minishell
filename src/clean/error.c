/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:52 by mwallage          #+#    #+#             */
/*   Updated: 2024/01/07 13:09:34 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_msg(const char *info)
{
	char	*errno_readable;

	write(STDERR_FILENO, "philoshell: ", 12);
	if (info)
		write(STDERR_FILENO, info, ft_strlen(info));
	errno_readable = strerror(errno);
	if (errno)
	{
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, errno_readable, ft_strlen(errno_readable));
	}
	write(STDERR_FILENO, "\n", 1);
	return (1);
}

int	syntax_error(const char *token, int *exitcode)
{
	write(STDERR_FILENO, "philoshell: ", 12);
	write(STDERR_FILENO, "syntax error near unexpected token '", 36);
	if (token && token[0] == '\n')
		write(STDERR_FILENO, "newline", 7);
	else if (token)
		write(STDERR_FILENO, token, ft_strlen(token));
	else
		write(STDERR_FILENO, "(NULL)", 6);
	write(STDERR_FILENO, "'\n", 2);
	*exitcode = 2;
	return (0);
}

int	redirect_error(t_group *group, const char *info)
{
	error_msg(info);
	if (group->pid == 0)
		cleanup_and_exit(group, 1);
	*group->exitcode = 1;
	return (0);
}
