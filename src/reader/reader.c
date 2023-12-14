/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:25 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/14 12:21:41 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	prompt_cat(char *prompt, char *user, char *path, char *home)
{
	int		len;

	len = ft_strlcpy(prompt, GRN_BOLD, ft_strlen(GRN_BOLD) + 1);
	len = ft_strlcat(prompt, user, len + ft_strlen(user) + 1);
	len = ft_strlcat(prompt, "@", len + 2);
	len = ft_strlcat(prompt, HOSTNAME, len + ft_strlen(HOSTNAME) + 1);
	len = ft_strlcat(prompt, RES, len + ft_strlen(RES) + 1);
	len = ft_strlcat(prompt, ":", len + 2);
	len = ft_strlcat(prompt, BLU_BOLD, len + ft_strlen(BLU_BOLD) + 1);
	if (ft_strncmp(path, home, ft_strlen(home)) == 0)
	{
		len = ft_strlcat(prompt, "~", len + 2);
		path += ft_strlen(home);
	}
	len = ft_strlcat(prompt, path, len + ft_strlen(path) + 1);
	len = ft_strlcat(prompt, " $ ", len + 4);
	ft_strlcat(prompt, RES, len + ft_strlen(RES) + 1);
}

static char	*get_prompt(char **env)
{
	char	*path;
	char	*prompt;
	char	*user;
	char	*home;

	user = mini_getenv(env, "USER");
	if (!user)
		user = "user";
	path = mini_getenv(env, "PWD");
	if (!path)
		path = "";
	home = mini_getenv(env, "HOME");
	if (!home)
		home = "";
	prompt = malloc(ft_strlen(user) + ft_strlen(HOSTNAME)
		+ ft_strlen(path) + 41);
	if (prompt == NULL)
		return (NULL);
	prompt_cat(prompt, user, path, home);
	return (prompt);
}

char	*reader(char **env)
{
	char	*str;
	char	*prompt;

	prompt = get_prompt(env);
	protect_malloc(NULL, prompt);
	// set_input_signals();
	str = readline(prompt);
	free(prompt);
	add_history(str);
	return (str);
}
