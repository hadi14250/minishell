/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 22:32:41 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/03 23:22:48 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	set_env_underscore(char *cmd, t_mini *mini)
{
	t_env	*tmp;

	tmp = mini->l_env;
	while (tmp)
	{
		if (ft_strncmp("_=", tmp->key, 2) && ft_strlen(tmp->key) == 1)
			break ;
		tmp = tmp->next;
	}
	if (tmp == NULL)
		return ;
	free (tmp->value);
	tmp->value = ft_strdup(cmd);
}

/**
 * @brief incriments shell lvl <SHLVL>
 * 
 * @param env 
 * @param mini 
 */

void	add_shlvl(t_env *env, t_mini *mini)
{
	t_env	*temp;
	t_env	*new;

	temp = env;
	new = ft_calloc(sizeof(t_env), 1);
	if (!new)
		ft_exit_shell(mini, 137, "Page allocation failure\n", 2);
	new->initialised = true;
	new->key = ft_strdup("SHLVL");
	new->value = ft_strdup("1");
	new->next = NULL;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

char	*return_shlvl(int lvl, t_mini *mini)
{
	char	*shlvl;

	shlvl = NULL;
	if (lvl == 999)
		shlvl = ft_strdup("");
	else if (lvl > 999)
		shlvl = ft_strdup("1");
	else if (lvl < 0)
		shlvl = ft_strdup("0");
	else
		shlvl = ft_itoa(++lvl);
	if (shlvl == NULL)
		ft_exit_shell(mini, 137, "Page allocation failure\n", 2);
	return (shlvl);
}

void	increase_shlvl(t_env *env, t_mini *mini)
{
	t_env	*tmp;
	int		lvl;
	bool	found;

	found = false;
	lvl = 0;
	tmp = env;
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->key, "SHLVL", 6) == 0)
		{
			found = true;
			lvl = ft_atoi(tmp->value);
			tmp->value = ft_free(tmp->value);
			tmp->value = return_shlvl(lvl, mini);
		}
		tmp = tmp->next;
	}
	if (!found)
		add_shlvl(env, mini);
}

/**
 * @brief Parses through the enviroment <envp> and allocates the keys
 * and their values into a singly linked list.
 * 
 * @param envp 
 * @return The enviromental linked list
 */

char	**add_basic_env(t_mini *mini)
{
	char	**envp;
	char	cwd[2056];
	char	*pwd;
	char	*full_env;

	ft_bzero(cwd, sizeof(cwd));
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		pwd = ft_strjoin("PWD=", cwd);
	else
		return (NULL);
	full_env = ft_strjoin(pwd, " SHLVL=0 _=/usr/bin/env");
	if (!full_env)
		return (NULL);
	free(pwd);
	envp = ft_split(full_env, ' ');
	if (!envp)
		ft_exit_shell(mini, 1, "add_basic_env", 2);
	free(full_env);
	return (envp);
}
