/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 18:27:45 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/03 23:18:18 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_parse_env(t_mini *mini, const char **envp)
{
	t_env	*env_new;
	t_env	*env_tail;

	if (!envp || !envp[0])
		envp = (const char **)add_basic_env(mini);
	env_tail = NULL;
	while (*envp)
	{
		env_new = (t_env *) ft_calloc(1, sizeof(t_env));
		if (!env_new)
			ft_exit_shell(mini, 137, "Page allocation failure\n", 2);
		env_new->key = ft_substr(*envp, 0, (ft_strchr(*envp, '=') - *envp));
		env_new->value = ft_strdup(ft_strchr(*envp++, '=') + 1);
		env_new->initialised = true;
		if (!env_new->key || !env_new->value)
			ft_exit_shell(mini, 137, "Page allocation failure\n", 2);
		env_new->next = NULL;
		if (mini->l_env == NULL)
			mini->l_env = env_new;
		else
			env_tail->next = env_new;
		env_tail = env_new;
	}
	increase_shlvl(mini->l_env, mini);
}
