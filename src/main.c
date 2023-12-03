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
	protect_malloc(envp_cpy);
	exitcode = 0;
	while (1)
	{
		line = reader(envp_cpy);
		tokens = tokenizer(line);
		if (!tokens)
		{
			free(envp_cpy);
			error_msg(MALLOC_MSG);
			exit(MALLOC_CODE);
		}
		if (!*tokens)
		{
			free(tokens);
			continue ;
		}
		list = parser(tokens, envp_cpy, exitcode);
		if (!list)
		{
			free(tokens);
			free(envp_cpy);
			error_msg(MALLOC_MSG);
			exit(MALLOC_CODE);
		}
		if (!expander(list))
		{
			cleanup(list);
			free(envp_cpy);
			error_msg(MALLOC_MSG);
			exit(MALLOC_CODE);
		}
		executor(list);
		envp_cpy = list->env;
		exitcode = group_last(list)->exitcode;
		cleanup(list);
	}
	free(envp_cpy);
	return (0);
}
