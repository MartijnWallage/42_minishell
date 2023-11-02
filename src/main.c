/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/01 14:33:09 by jmuller          ###   ########.fr       */
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
		tokens = lexer(line);
		free(line);
		if (!tokens)
			handle_error(MALLOC_MSG, MALLOC_CODE);
		if (!*tokens)
		{
			free(tokens);
			continue ;
		}
		list = parser(tokens, envp);
		expander(list);
		executor(list);
		cleanup(list);
	}
	return (0);
}
