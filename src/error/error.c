/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:52 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/22 18:14:54 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_error(char *info, int exitcode)
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
	len = ft_strlen(info);
	write(STDERR_FILENO, info, len);
	write(STDERR_FILENO, "\n", 1);
	exit(exitcode);
}
