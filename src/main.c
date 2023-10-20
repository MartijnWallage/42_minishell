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

#include "../inc/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_token		*tokens;
	t_branch	*tree;
	pid_t		pid;

	if (argc != 1 || argv[1])
		return (1);
	while (1)
	{
		line = reader();
		if (!line)
			continue ;
		if (ft_strncmp(line, "exit", 4) == 0)
		{
			free(line);
			break ;
		}
		tokens = lexer(line);
		tree = parser(tokens, envp);
		executor(tree);
		cleanup(tree);
	}
	return (0);
}
