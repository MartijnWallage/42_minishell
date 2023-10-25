/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/25 18:15:34 by mwallage         ###   ########.fr       */
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
			handle_error("no input", 1);
		tokens = split_line(line);
		free(line);
		if (!tokens)
			handle_error(MALLOC_MSG, MALLOC_CODE);
		if (!*tokens)
		{
			free_tab(tokens);
			continue ;
		}
  		list = parser(tokens, envp);
		executor(list);
		cleanup(list); 
	}
	return (0);
}
