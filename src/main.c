/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/13 18:16:34 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**tokens;
	t_group	*list;

	if (argc != 1 || argv[1])
		return (1);
	while (1)
	{
		line = reader();
		if (!line)
		{
			perror("philoshell: no input");
			return (1);
		}
		tokens = split_line(line);
		free(line);
 		list = parser(tokens, envp);
		executor(list); 
		cleanup(list);
	}
	return (0);
}
