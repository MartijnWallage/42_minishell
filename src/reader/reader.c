/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:25 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/19 11:51:36 by mwallage         ###   ########.fr       */
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

static char *get_prompt(void)
{
	char	*current_path;
	char	*prompt;
	char	*user;
	char	*logname;
	char	*home;
	int		len;
	char	buffer[1024];

	user = getenv("USER");
	logname = getenv("LOGNAME");
	current_path = getcwd(buffer, 1024);
	home = getenv("HOME");
	if (ft_strnstr(current_path, home, ft_strlen(current_path)))
		current_path += ft_strlen(home);
	prompt = malloc(ft_strlen(user) + ft_strlen(home) + ft_strlen(current_path) + 7);
	if (prompt == NULL)
		return (NULL);
	len = ft_strlcpy(prompt, user, ft_strlen(user) + 1);
	len = ft_strlcat(prompt, "@", len + 2);
	len = ft_strlcat(prompt, logname, len + ft_strlen(logname) + 1);
	len = ft_strlcat(prompt, ":", len + 2);
	if (ft_strncmp(current_path - ft_strlen(home), home, ft_strlen(home)) == 0)
		len = ft_strlcat(prompt, "~", len + 2);
	len = ft_strlcat(prompt, current_path, len + ft_strlen(current_path) + 1);
	ft_strlcat(prompt, " $ ", len + 4);
	return (prompt);
}

char	*reader(void)
{
	char	*str;
	char	*prompt;
	
	prompt = get_prompt();	
	str = readline(prompt);
	free(prompt);
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
