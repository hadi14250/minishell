/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_evaltokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:26:09 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/06 01:49:12 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

size_t	ft_evalvar(t_env *env)
{
	char	**split;
	size_t	word_count;

	word_count = 0;
	if (!env || !env->initialised)
		return (word_count);
	if (env && env->initialised)
	{
		split = ft_split(env->value, ' ');
		while (split[word_count])
			word_count++;
		ft_free_split(split);
	}
	return (word_count);
}

bool	ft_evalfile(t_token *current, t_mini *mini, bool heredoc_flag)
{
	bool	valid;
	size_t	count;
	t_env	*env;

	valid = false;
	while (current && current->type != SPACES && current->type != PIPE \
	&& current->type != REDIRECTION)
	{
		if (heredoc_flag)
			current->type = WORD;
		if (current->type == VARIABLE)
		{
			env = env_already_exist(current->content + 1, mini);
			count = ft_evalvar(env);
			if (count > 1)
				return (ft_syntaxerr("ambiguous redirect", 1, false));
			else
				valid |= count;
		}
		else
			valid = true;
		current = current->next;
	}
	return (ft_syntaxerr("ambiguous redirect", 1, valid));
}

bool	ft_evalrdr(t_token *head, t_mini *mini)
{
	t_token	*current;
	bool	heredoc_flag;

	heredoc_flag = 0;
	if (!ft_strncmp(head->content, "<<", 3))
		heredoc_flag = true;
	current = head->next;
	if (!current)
		return (ft_syntaxerr("syntax error near unexpected token", 258, 0));
	if (current->type == SPACES)
		current = current->next;
	if (!current || current->type == PIPE || current->type == REDIRECTION)
		return (ft_syntaxerr("syntax error near unexpected token", 258, 0));
	if (!ft_evalfile(current, mini, heredoc_flag))
		return (false);
	return (true);
}

bool	ft_evalops(t_token *head, t_mini *mini)
{
	t_token	*current;

	current = head;
	if (current->type == PIPE)
	{
		if (!current->prev)
			return (ft_syntaxerr("syntax error near unexpected token", 258, 0));
		current = head->next;
		if (!current)
			return (ft_syntaxerr("syntax error near unexpected token", 258, 0));
		if (current->type == SPACES)
			current = current->next;
		if (!current || current->type == PIPE)
			return (ft_syntaxerr("syntax error near unexpected token", 258, 0));
		return (true);
	}
	else
		return (ft_evalrdr(current, mini));
}

bool	ft_evaltokens(t_mini *mini)
{
	t_token	*current;

	current = mini->l_token;
	while (current)
	{
		if (current->type == SINGLE || current->type == DOUBLE)
			if (!ft_strchr(current->content + 1, *current->content))
				return (ft_syntaxerr("syntax error near unexpected token", \
				258, false));
		current = current->next;
	}
	current = mini->l_token;
	while (current)
	{
		if (current->type == PIPE || current->type == REDIRECTION)
			if (!ft_evalops(current, mini))
				return (false);
		current = current->next;
	}
	return (true);
}
