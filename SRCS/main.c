/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 16:49:59 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/06 01:46:17 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	take_input(t_mini *mini)
{
	signal(SIGINT, &ft_interupt);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		mini->rl = read_line_colored(random_between(1, 6));
		if (mini->rl == NULL)
			ft_exit_shell(mini, g_exit_code, "exit", 1);
		if (mini->rl[0] != '\0')
			add_history(mini->rl);
		ft_tokenize(mini);
		if (mini->l_token)
			ft_parse_token(mini);
		if (mini->l_cmd)
			parse_input(mini);
		ft_free_cycle(mini);
	}
}

void	print_wlcm_msg(t_mini *mini)
{
	char	*user;

	user = find_str_env("USER", mini, VALUE);
	if (!user)
	{
		fd_printf(1, WLC""WLC2""WLC3""WLC4""WLC5,
			ft_atoi(find_str_env("SHLVL", mini, VALUE)) \
		, "unkown 🕵");
	}
	else
	{
		fd_printf(1, WLC""WLC2""WLC3""WLC4""WLC5,
			ft_atoi(find_str_env("SHLVL", mini, VALUE)) \
		, user);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void) argc;
	(void) argv;
	ft_bzero(&mini, sizeof(t_mini));
	ft_parse_env(&mini, (const char **)envp);
	print_wlcm_msg(&mini);
	take_input(&mini);
}
