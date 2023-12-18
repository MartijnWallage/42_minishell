/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:16:11 by jmuller           #+#    #+#             */
/*   Updated: 2023/12/18 17:36:29 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sigid;

void	display_prompt(int sig)
{
	(void) sig;
	write(STDOUT_FILENO, "\n", 2);
	rl_on_new_line();
	rl_redisplay();
	rl_replace_line("", 0);
}

void	interrupt_child(int sig)
{
	write(STDOUT_FILENO, "\n", 1);
	g_sigid = SIGINT;
	(void) sig;
}

void	quit_child(int sig)
{
	write(STDERR_FILENO, "Quit (core dumped)\n",
		ft_strlen("Quit (core dumped)\n)"));
	g_sigid = SIGQUIT;
	(void) sig;
}

void	interrupt_heredoc(int sig)
{
	g_sigid = SIGINT;
	(void) sig;
	write(STDERR_FILENO, "\n", 1);
	exit(130);
}

void	handle_signals(t_mode mode)
{
	g_sigid	= 0;
	if (mode == NON_INTERACTIVE)
	{
		signal(SIGINT, interrupt_child);
		signal(SIGQUIT, quit_child);	
	}
	else if (mode == INTERACTIVE)
	{
		signal(SIGINT, display_prompt);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == HEREDOC)
	{
		signal(SIGINT, interrupt_heredoc);
	}
}

void	check_signal_flag(t_group *group)
{
	if (!group || g_sigid == 0 || group->pid <= 0)
		return ;
	if (g_sigid == SIGINT)
		*group->exitcode = 130;
	if (g_sigid == SIGQUIT)
		*group->exitcode = 131;
}
