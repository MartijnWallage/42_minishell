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
	t_var	*vars;
	char	**tokens;
	t_group	*list;

	if (argc != 1 || argv[1])
		return (1);
	while (1)
	{
		vars = malloc(sizeof(t_var));
		if (!vars)
			handle_error(MALLOC_MSG, MALLOC_CODE);	// exits directly, not allowed
		vars->line = reader();
		if (!vars->line)
			handle_error("No input", 1);			// exits directly, not allowed
		if (ft_strncmp(vars->line, "exit", 4) == 0)
		{
			free_vars(vars);
			break ;
		}
		vars->env = envp;
		tokens = lexer(vars->line);
 		list = parser(tokens, envp);
		executor(list); 
		cleanup(vars, list);
	}
	return (0);
}
