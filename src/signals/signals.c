/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:16:11 by jmuller           #+#    #+#             */
/*   Updated: 2023/12/14 14:20:24 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_flag = 0;

void	display_prompt(int sig)
{
	(void) sig;
	write(1, "\n", 2);
	rl_on_new_line();
	rl_redisplay();
	rl_replace_line("", 0);
	g_flag = 1;
}

void	signal_handler_parent(void)
{
	signal(SIGINT, display_prompt);
	signal(SIGQUIT, SIG_IGN);
}

void	check_signal_flag(t_group *group)
{
	if (g_flag && group->pid > 0)
	{
		*group->exitcode = 130;
		write(2, "Quit (core dumped)\n", ft_strlen("Quit (core dumped)\n"));
	}
}
