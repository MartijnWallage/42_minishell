/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:30 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 14:38:11 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirect(const char *str)
{
	if (str == NULL)
		return (0);
	if (ft_strncmp(str, "<<", 3) == 0
		|| ft_strncmp(str, "<", 2) == 0
		|| ft_strncmp(str, ">>", 3) == 0
		|| ft_strncmp(str, ">", 2) == 0)
		return (1);
	return (0);
}
