/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:25 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/16 15:21:53 by jmuller          ###   ########.fr       */
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

/* const char *prompt(char **str)
{
	int		i;
	char	*res;
	char	*temp;
	char	*temp2;
	
	i = 0;
	while (str[i])
	{
		if (ft_strncmp(str[i], "LOGNAME", ft_strlen("LOGNAME")) == 0)
		{
			res = ft_strdup(get_value(str[i]));
		}
		i++;
	}
	i = 0;
	temp = ft_strjoin_safe(res, "@");
	free(res);
	res = temp;
	while (str[i])
	{
		if (ft_strncmp(str[i], "NAME", ft_strlen("NAME")) == 0)
		{
			temp = ft_strdup(get_value(str[i]));
			temp2 = ft_strjoin_safe(res, temp);
			free(res);
			free(temp);
			res = temp2;
		}
		i++;
	}
	temp = ft_strjoin_safe(res, ":");
	free(res);
	res = temp;
	i = 0;
	while (str[i])
	{
		if (ft_strncmp(str[i], "PWD", ft_strlen("PWD")) == 0)
		{
			temp = ft_strdup(get_value(str[i]));
			temp2 = ft_strjoin_safe(res, temp);
			free(res);
			free(temp);
			res = temp2;
		}
		i++;
	}
	return (res);
}
 */

char	*reader(void)
{
	char	*str;
	
	str = readline(PROMPT);	// prompt should display username, login name : pwd
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
