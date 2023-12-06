/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/03 23:37:14 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**tokens;
	char	**env_cpy;
	int		exitcode;
	t_group	*list;

	if (argc != 1 || argv[1])
		return (1);
	env_cpy = envp;
	exitcode = 0;
	while (1)
	{
		line = reader(env_cpy);
		if (!line)
		{
			error_msg("readline failed");
			rl_clear_history();
			exit(errno);
			// echo hello && ls && cat README.md | wc || echo failed!
		}
		tokens = tokenizer(line);
		free(line);
		if (!tokens)
		{
			rl_clear_history();
			if (env_cpy != envp)
				free_tab(env_cpy);
			error_msg(MALLOC_MSG);
			return (MALLOC_CODE);
		}
		list = parser(tokens, env_cpy, exitcode);
		if (env_cpy != envp)
			free_tab(env_cpy);
		expander(list);
		executor(list);
		env_cpy = copy_tab(group_last(list)->env);
		protect_malloc(list, env_cpy);
		exitcode = group_last(list)->exitcode;
		cleanup(list);
	}
	return (0);
}
