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

static int	is_valid_input(char *input)
{
	if (!input)
		return (0);
	if (*input == '?') // just to test for now
	{
		ft_printf("%s\n", INVALID_INPUT);
		free(input);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	pid_t	pid;

	if (argc != 1)
		return (1);
	argv++;	// compiler forces me to do something with argv
	while (1)
	{
		line = readline(PROMPT);
		if (!is_valid_input(line))
			continue ;
		// branch(line);
		// descend_tree(tree);
		pid = fork();
		if (pid == 0)
			exec(line, envp);
		else // if !(command ends on &)
			waitpid(pid, NULL, 0);
		free(line);
	}

	return (0);
}
