/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:30 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/16 15:09:09 by jmuller          ###   ########.fr       */
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

void	*free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

char	**copy_tab(char **tab)
{
	int		size;
	int		i;
	char	**ret;

	size = tab_len(tab);
	ret = malloc(size * (sizeof(char *) + 1));
	// protect malloc
	ret[size] = NULL;
	i = -1;
	while (++i < size)
		ret[i] = ft_strdup(tab[i]);
	return (ret);
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
	char **kv_pair;
	char *copy;
	
	kv_pair = ft_split(str, '=');
	copy = ft_strdup(kv_pair[0]);
	free_tab(kv_pair);
	return (copy);
}

char	*get_value(char *str)
{
	char **kv_pair;
	char *copy;
	
	kv_pair = ft_split(str, '=');
	copy = ft_strdup(kv_pair[1]);
	free_tab(kv_pair);
	return (copy);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	char	*result;

	if (s1 == NULL)
		return ((char *)s2);
	if (s2 == NULL)
		return ((char *)s1);
	joined = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (joined == NULL)
		return (NULL);
	result = joined;
	while (*s1)
		*joined++ = *s1++;
	while (*s2)
		*joined++ = *s2++;
	*joined = '\0';
	return (result);
}

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