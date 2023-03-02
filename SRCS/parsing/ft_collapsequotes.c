/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_collapsequotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:26:09 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/06 03:28:09 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_addmidstrerror(char *start, size_t i)
{
	char	*result;
	char	*hold;

	hold = ft_itoa(g_exit_code);
	result = ft_strjoin(hold, start + i);
	hold = (char *) ft_free(hold);
	return (result);
}

char	*ft_addmidstr(char *start, t_env *match, size_t i, t_mini *mini)
{
	char	*result;

	if (match && match->initialised)
		result = ft_strjoin(match->value, start + i);
	else
		result = ft_strdup(ft_memmove(start, start + i, \
		ft_strlen(start + i) + 1));
	if (!result)
		ft_exit_shell(mini, 137, "Page allocation failure", 2);
	return (result);
}

char	*ft_str_expand(char *start, char *quote, t_mini *mini, size_t i)
{
	char				*temp;
	char				*hold;

	temp = NULL;
	hold = ft_substr(start + 1, 0, i - 1);
	if (*(start + 1) == '?')
		temp = ft_addmidstrerror(start, i);
	else
		temp = ft_addmidstr(start, env_already_exist(hold, mini), i, mini);
	hold = (char *) ft_free(hold);
	*start = '\0';
	start = ft_strjoin(quote, temp);
	temp = (char *) ft_free(temp);
	quote = (char *) ft_free(quote);
	quote = start;
	return (quote);
}

char	*ft_check_var(char *quote, t_mini *mini)
{
	char				*start;
	enum e_tokentype	type;
	size_t				i;

	start = ft_strchr(quote, '$');
	while (start)
	{
		i = 1;
		type = VARIABLE;
		ft_eovchr(&i, start + 1, &type);
		if (type == VARIABLE)
		{
			quote = ft_str_expand(start, quote, mini, i);
			start = ft_strchr(quote, '$');
		}
		else
			start = ft_strchr(start + 1, '$');
	}
	return (quote);
}

void	ft_collapsequotes(t_mini *mini)
{
	t_token	*current;

	current = mini->l_token;
	while (current)
	{
		if (current->type == SINGLE || current->type == DOUBLE)
		{
			ft_memmove(current->content, current->content + 1, \
			(ft_strlen(current->content) - 2));
			current->content[ft_strlen(current->content) - 2] = '\0';
			if (current->type == DOUBLE)
				current->content = ft_check_var(current->content, mini);
		}
		current = current->next;
	}
}
