/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/24 15:54:18 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**tokens;
	char	**envp_cpy;
	int		exitcode;
	t_group	*list;

	if (argc != 1 || argv[1])
		return (1);
	envp_cpy = copy_tab(envp);
	exitcode = 0;
	while (1)
	{
		line = reader(envp_cpy);
		tokens = lexer(line);
		free(line);
		if (!tokens)
			err_and_exit(MALLOC_MSG, MALLOC_CODE);
		if (!*tokens)
		{
			free(tokens);
			continue ;
		}
		list = parser(tokens, envp_cpy, exitcode);
		if (!list)
		{
			free(tokens);	// this isn't quite right. Need a consistent policy for freeing *tokens.
			err_and_exit(MALLOC_MSG, MALLOC_CODE);
		}
		expander(list);
		executor(list);
		envp_cpy = list->env;
		exitcode = group_last(list)->exitcode;
		cleanup(list);
	}
	return (0);
}
