/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:25 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/18 18:41:52 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_input(char *input)
{
	if (*input == '?') // just to test for now
	{
		ft_printf("%s\n", INVALID_INPUT);
		return (0);
	}
	return (1);
}

char	*get_path_right_of_home(char *path, char *home)
{
	char	*path_right_of_home;
	
	path_right_of_home = ft_substr(path, ft_strlen(home), ft_strlen(path));
	free(path);
	return (path_right_of_home);
}

/*	We should use a version of strjoin
	that always frees the first given argument */
char *get_prompt(char **env)
{
	char	*user;
	char	*logname;
	char	*current_path;
	char	*prompt;
	char	*home;

 	user = get_value(ft_grep(env, "USER="));
	logname = get_value(ft_grep(env, "LOGNAME="));
	current_path = get_value(ft_grep(env, "PWD="));
	prompt = ft_strjoin(user, "@");
	prompt = ft_strjoin(prompt, logname);
	prompt = ft_strjoin(prompt, ":");
	home = get_value(ft_grep(env, "HOME="));
	if (ft_strnstr(current_path, home, ft_strlen(current_path)))
	{
		prompt = ft_strjoin(prompt, "~");
		current_path = get_path_right_of_home(current_path, home);
	}
	prompt = ft_strjoin(prompt, current_path);
	prompt = ft_strjoin(prompt, " $ ");
	free(home);
	free(user);
	free(logname);
	free(current_path);
	return (prompt);
}

char	*reader(const char *prompt)
{
	char	*str;
	
	str = readline(prompt);
	if (!str)
		return (NULL);
	add_history(str);
	if (!is_valid_input(str))
	{
		free(str);
		return (NULL);
	}
	return (str);
}
