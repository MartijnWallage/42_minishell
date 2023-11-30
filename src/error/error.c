/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:52 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/23 16:24:41 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_msg(char *info)
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
		return ;
	len = ft_strlen(info);
	write(STDERR_FILENO, info, len);
	write(STDERR_FILENO, "\n", 1);
}

void	protect_malloc(void *ptr)
{
	if (ptr == NULL)
	{
		error_msg(MALLOC_MSG);
		exit(MALLOC_CODE);
	}
}