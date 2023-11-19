/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:25 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/19 12:28:45 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *get_prompt(char **env)
{
	char	*path;
	char	*prompt;
	char	*user;
	char	*logname;
	char	*home;
	int		len;

	user = mini_getenv(env, "USER");
	logname = mini_getenv(env, "LOGNAME");
	path = mini_getenv(env, "PWD");
	home = mini_getenv(env, "HOME");
	prompt = malloc(ft_strlen(user) + ft_strlen(home) + ft_strlen(path) + 7);
	if (prompt == NULL)
		return (NULL);
	len = ft_strlcpy(prompt, user, ft_strlen(user) + 1);
	len = ft_strlcat(prompt, "@", len + 2);
	len = ft_strlcat(prompt, logname, len + ft_strlen(logname) + 1);
	len = ft_strlcat(prompt, ":", len + 2);
	if (ft_strncmp(path, home, ft_strlen(home)) == 0)
	{
		len = ft_strlcat(prompt, "~", len + 2);
		path += ft_strlen(home);
	}
	len = ft_strlcat(prompt, path, len + ft_strlen(path) + 1);
	ft_strlcat(prompt, " $ ", len + 4);
	return (prompt);
}

char	*reader(char **env)
{
	char	*str;
	char	*prompt;
	
	prompt = get_prompt(env);
	if (!prompt)
		handle_error(MALLOC_MSG, MALLOC_CODE);	
	str = readline(prompt);
	free(prompt);
	add_history(str);
	return (str);
}
