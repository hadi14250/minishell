/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expandvar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:26:09 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/05 03:16:55 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_init_splitvar(t_mini *mini, t_token **current, char **split)
{
	t_token	*tmp;
	size_t	i;

	i = 1;
	while (split[i])
	{
		tmp = (t_token *) ft_calloc(1, sizeof(t_token));
		if (!tmp)
			ft_exit_shell(mini, 137, "Page allocation failure", 2);
		tmp->type = SPACES;
		add_node_middle((*current), tmp);
		(*current) = (*current)->next;
		tmp = (t_token *) ft_calloc(1, sizeof(t_token));
		if (!tmp)
			ft_exit_shell(mini, 137, "Page allocation failure", 2);
		tmp->content = ft_strdup(split[i++]);
		if (!tmp->content)
		{
			tmp = (t_token *) ft_free(tmp);
			ft_exit_shell(mini, 137, "Page allocation failure", 2);
		}
		tmp->type = WORD;
		add_node_middle((*current), tmp);
		(*current) = (*current)->next;
	}
}

void	ft_splitvar(t_mini *mini, t_token **current)
{
	char	**split;

	split = ft_split((*current)->content, ' ');
	if (!split)
		ft_exit_shell(mini, 137, "Page allocation failure", 2);
	if (*split)
	{
		(*current)->content = (char *) ft_free((*current)->content);
		(*current)->content = ft_strdup(*split);
		ft_init_splitvar(mini, current, split);
	}
	else
		(*current)->content[0] = '\0';
	split = (char **) ft_free_split(split);
}

void	ft_expandvar(t_mini *mini)
{
	t_token	*current;
	t_env	*l_env;

	current = mini->l_token;
	while (current)
	{
		if (current->type == VARIABLE)
		{
			l_env = env_already_exist(current->content + 1, mini);
			current->content[0] = '\0';
			if ((l_env && l_env->initialised) || *(current->content + 1) == '?')
			{
				current->content = (char *) ft_free(current->content);
				if (l_env)
					current->content = ft_strdup(l_env->value);
				else
					current->content = ft_itoa(g_exit_code);
			}
			current->type = WORD;
			ft_splitvar(mini, &current);
		}
		current = current->next;
	}
}
