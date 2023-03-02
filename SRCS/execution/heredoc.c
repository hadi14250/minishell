/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 19:27:57 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/06 01:32:35 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	null_params(char **tmp, char **total, char **line)
{
	*tmp = NULL;
	*total = NULL;
	*line = NULL;
}

void	free_params(char **line, char **tmp, char **input)
{
	*line = ft_free(*line);
	*tmp = ft_free(*tmp);
	*input = ft_free(*input);
}

void	exit_heredoc(t_mini *mini, t_rdr *rdr, char *total)
{
	rdr->herepipe[0] = ft_close(rdr->herepipe[0], 0, NULL);
	rdr->herepipe[1] = ft_close(rdr->herepipe[1], 0, NULL);
	total = ft_free(total);
	ft_exit_shell(mini, 69, NULL, 2);
}

void	take_heredoc_input(t_rdr *rdr, t_mini *mini)
{
	t_hrdoc	h;

	signal(SIGINT, &ft_intheredoc);
	null_params(&h.tmp, &h.total, &h.line);
	h.quoted = parse_delimiter(rdr);
	while (1)
	{
		h.i = readline("> ");
		if (g_exit_code == 69)
		{
			exit_heredoc(mini, rdr, h.total);
			free_params(&h.line, &h.tmp, &h.i);
		}
		if (!h.i || !ft_strncmp(h.i, rdr->file, ft_strlen(rdr->file) + 1))
			break ;
		h.tmp = h.line;
		h.line = gl_strjoin(h.i, "\n");
		if (!h.quoted)
			h.line = ft_check_var(h.line, mini);
		h.tmp = ft_free(h.tmp);
		h.tmp = h.total;
		h.total = gl_strjoin(h.total, h.line);
		free_params(&h.line, &h.tmp, &h.i);
	}
	send_input_to_parent(mini, h.i, rdr, h.total);
}

int	handle_heredoc(t_mini *mini)
{
	t_cmd	*cmd;
	t_rdr	*rdr;
	int		ret;

	ret = 0;
	cmd = mini->l_cmd;
	while (cmd)
	{
		rdr = cmd->rdr;
		if (rdr)
		{
			while (rdr)
			{
				if (rdr->e_rdr == HEREDOC)
				{
					ret = take_input_from_child(mini, rdr);
					if (ret != 0)
						return (ret);
				}
				rdr = rdr->next;
			}
		}
		cmd = cmd->next;
	}
	return (0);
}
