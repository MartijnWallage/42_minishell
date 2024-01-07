/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:25 by mwallage          #+#    #+#             */
/*   Updated: 2024/01/07 10:19:06 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_home_in_path(const char *home, const char *path)
{
	while (*path && *home == *path)
	{
		home++;
		path++;
	}
	if (*home == 0 && (*path == 0 || *path == '/'))
		return (1);
	return (0);
}

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
	if (ft_strlen(home) > 0 && is_home_in_path(home, path))
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
	char	buffer[1024];

	user = mini_getenv(env, "USER");
	if (!user)
		user = "user";
	path = mini_getenv(env, "PWD");
	if (!path)
		path = getcwd(buffer, 1024);
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
/**
 * @brief Reads user input after displaying a customized prompt
 *
 * This function 
 * 1. Creates a prompt using env.
 * 2. Reads user input continuously and returns it as a string.
 * 3. Handles signals for interactive input.
 * 4. Updates the command history if the input is not empty.
 * @param env Pointer to the array of environment variables.
 * @return Returns the line of input read from user. 
 */

char	*reader(char **env)
{
	char	*str;
	char	*prompt;

	prompt = get_prompt(env);
	protect_malloc(NULL, prompt);
	handle_signals(INTERACTIVE);
	str = readline(prompt);
	free(prompt);
	if (str && *str)
		add_history(str);
	return (str);
}
