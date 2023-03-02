/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:26:09 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/05 03:14:50 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_type	ft_label_token(char c)
{
	if (c == ' ' || c == '\t')
		return (SPACES);
	if (c == '\'')
		return (SINGLE);
	if (c == '\"')
		return (DOUBLE);
	if (c == '$')
		return (VARIABLE);
	if (c == '<' || c == '>')
		return (REDIRECTION);
	if (c == '|')
		return (PIPE);
	return (WORD);
}

void	ft_eovchr(size_t *i, const char *str, enum e_tokentype *type)
{
	if (!ft_isalpha(*str) && *str != '\'' && *str != '\"' \
	&& *str != '_' && *str != '?')
		*type = WORD;
	if (*str != '?')
		while (str[*i - 1] && (ft_isalnum(str[*i - 1]) || str[*i - 1] == '_'))
			*i = *i + 1;
	else
		*i = 2;
}

bool	ft_syntaxerr(char *errmsg, int num, bool ret)
{
	if (ret == false)
	{
		fd_printf(2, "minishell: %s\n", errmsg);
		g_exit_code = num;
		return (ret);
	}
	else
		return (true);
}
