/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer copy.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:26:09 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/05 02:20:15 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_next_token(t_mini *mini, t_token *new, char *line)
{
	enum e_tokentype	type;
	size_t				i;

	type = ft_label_token(*line);
	i = 1;
	if (type == VARIABLE)
		ft_eovchr(&i, line + 1, &type);
	if (type == WORD || type == SPACES)
		while (line[i] && ft_label_token(line[i]) == type)
			i++;
	if (type == REDIRECTION && *line == line[i])
		i++;
	if (type == SINGLE || type == DOUBLE)
		i = (unsigned long) ft_strchr(line + 1, *line) \
		- (unsigned long) line + 1;
	new->type = type;
	new->content = ft_substr(line, 0, i);
	if (!new->content)
		ft_exit_shell(mini, 137, "Page allocation failure", 2);
}

void	ft_tokenlist(t_mini *mini)
{
	t_token	*new;
	t_token	*prev;
	size_t	offset;

	offset = 0;
	prev = NULL;
	while (*(mini->rl + offset))
	{
		new = (t_token *) ft_calloc(1, sizeof(t_token));
		if (!new)
			ft_exit_shell(mini, 137, "Page allocation failure", 2);
		ft_next_token(mini, new, (mini->rl + offset));
		offset += ft_strlen(new->content);
		new->prev = prev;
		if (prev)
			prev->next = new;
		if (!mini->l_token)
			mini->l_token = new;
		prev = new;
	}
}
