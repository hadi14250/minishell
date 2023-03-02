/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:26:09 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/05 03:09:48 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_tokenize(t_mini *mini)
{
	ft_tokenlist(mini);
	if (!ft_evaltokens(mini))
	{
		ft_free_ltoken(mini->l_token);
		mini->l_token = NULL;
		return ;
	}
	ft_expandvar(mini);
	ft_collapsequotes(mini);
	join_same_type_nodes(mini->l_token);
	delete_empty_nodes(&(mini->l_token));
}
