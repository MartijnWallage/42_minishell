/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:52 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 22:14:36 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_msg(const char *info)
{
	char	*errno_readable;
	int		len;

	errno_readable = strerror(errno);
	len = ft_strlen(errno_readable);
	write(STDERR_FILENO, "philoshell: ", 12);
	if (errno)
	{
		write(STDERR_FILENO, errno_readable, len);
		write(STDERR_FILENO, ": ", 2);
	}
	if (info == NULL)
		return (0);
	len = ft_strlen(info);
	write(STDERR_FILENO, info, len);
	write(STDERR_FILENO, "\n", 1);
	return (0);
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
