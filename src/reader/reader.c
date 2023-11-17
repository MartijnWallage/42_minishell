/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:25 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/17 23:00:59 by mwallage         ###   ########.fr       */
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

char	*get_home_path(char *path, char *username)
{
	int		len;
	char	*home_path;
	int		i;

	len = ft_strlen(path);
	i = 0;
	while (path[i] && !ft_match(&path[i], username, len))
		i++;
	if (path[i] == 0)
		return (NULL);
	home_path = ft_substr(path, i + ft_strlen(username), len);
	return (home_path);
}

/*	We should use a version of strjoin
	that always frees the first given argument */
char *get_prompt(char **env)
{
	char	*user;
	char	*logname;
	char	*current_path;
	char	*prompt;

 	user = get_value(ft_grep(env, "USER="));
	logname = get_value(ft_grep(env, "LOGNAME="));
	current_path = get_value(ft_grep(env, "PWD="));
	prompt = ft_strjoin(user, "@");
	prompt = ft_strjoin(prompt, logname);
	prompt = ft_strjoin(prompt, ":");
	if (ft_strnstr(current_path, user, ft_strlen(current_path)))
	{
		prompt = ft_strjoin(prompt, "~");
		current_path = get_home_path(current_path, user);
	}
	prompt = ft_strjoin(prompt, current_path);
	prompt = ft_strjoin(prompt, " $ ");
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
