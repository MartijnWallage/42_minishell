/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:04 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/22 18:15:05 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**lexer(char *line)
{
	char	**tab;

	tab = ft_split(line, ' ');
	// take care of quotes etc.
	return (tab);
}
