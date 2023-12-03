/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/03 19:27:51 by mwallage         ###   ########.fr       */
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
	env_cpy = copy_tab(envp);
	protect_malloc(NULL, env_cpy);
	exitcode = 0;
	while (1)
	{
		line = reader(env_cpy);
		tokens = tokenizer(line);
		if (!tokens)
			return (free(env_cpy), error_msg(MALLOC_MSG), MALLOC_CODE);
		if (!*tokens)
		{
			free(tokens);
			continue ;
		}
		list = parser(tokens, env_cpy, exitcode);
		expander(list);
		executor(list);
		env_cpy = list->env;
		exitcode = group_last(list)->exitcode;
		cleanup(list);
	}
	return (0);
}
