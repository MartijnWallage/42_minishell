/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:30 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/03 19:52:07 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tab_len(char **tab)
{
	int	size;
	
	size = 0;
	while (tab && tab[size])
		size++;
	return (size);
}

char	**copy_tab(char **tab)
{
	int		size;
	int		i;
	char	**ret;

	size = tab_len(tab);
	ret = malloc(size * (sizeof(char *) + 1));
	if (!ret)
		return (NULL);
	ret[size] = NULL;
	i = -1;
	while (++i < size)
	{
		ret[i] = ft_strdup(tab[i]);
		if (!ret[i])
		{
			free_tab(tab);
			return (NULL);
		}
	}
	return (ret);
}

char	*mini_getenv(char **env, char *key)
{
	int	i;
	int	j;

	if (!env)
		return (NULL);
	if (!key)
		return (env[0]);
	i = -1;
	while (env[++i])
	{
		j = 0;
		while (env[i][j] && env[i][j] == key[j])
			j++;
		if (key[j] == 0 && env[i][j] == '=')
			return (&env[i][j + 1]);
	}
	return (NULL);
}

t_group	*group_last(t_group *group)
{
	t_group	*current;
	
	current = group;
	while (current && current->next)
		current = current->next;
	return (current);
}

int	key_compare(char **env, char *line)
{
	char	*key;
	int		i;

	i = 0;
	key = get_key(line);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0)
		{
			free(key);
			return (1);
		}
		i++;
	}
	free(key);
	return (0);
}

char	*get_key(char *str)
{
	int	i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
		return (NULL);
	i++;
	return (ft_substr(str, 0, i));
}

/* char	*get_value(char *str)
{	
	if (!str)
		return (NULL);
	while (*str && *str != '=')
		str++;
	if (!*str)
		return (NULL);
	str++;
	return (ft_strdup(str));
} */

char	*ft_strjoin_safe(char const *s1, char const *s2)
{
	char	*joined;
	char	*result;
	size_t len1;
	size_t len2;

	len1 = 0;
	len2 = 0;
	if (s1 != NULL)
		len1 = ft_strlen(s1);
	if (s2 != NULL)
		len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (joined == NULL)
		return (NULL);
	result = joined;
	if (s1 != NULL)
	{
		while (*s1)
			*joined++ = *s1++;
	}
	if (s2 != NULL) 
	{
		while (*s2)
			*joined++ = *s2++;
	}
	*joined = '\0';
	return (result);
}