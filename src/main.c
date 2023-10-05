/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/05 17:31:33 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	if (argc != 1)
		return (1);
	while (1)
	{
		line = readline(PROMPT);
		argv = ft_split(line, ' ');
		free(line);
		argc = 0;
		while (argv[argc])
			argc++;
		pipex(argc, argv, envp);
		free_tab(argv);
	}
	return (0);
}
