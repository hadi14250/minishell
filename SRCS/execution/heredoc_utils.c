/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 20:54:27 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/06 00:37:47 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	parse_delimiter(t_rdr *rdr)
{
	char	*tmp;
	char	hold;
	bool	quoted;

	quoted = false;
	tmp = rdr->file;
	while (1)
	{
		while (*tmp && *tmp != '\'' && *tmp != '\"')
			tmp++;
		if (!*tmp || !(ft_strchr(tmp + 1, *tmp)))
			return (quoted);
		hold = *tmp;
		ft_memmove(tmp, tmp + 1, ft_strlen(tmp));
		tmp = ft_strchr(tmp, hold);
		ft_memmove(tmp, tmp + 1, ft_strlen(tmp));
		quoted = true;
	}
	printf("%s\n", rdr->file);
	return (quoted);
}

void	trim_last_new_line(char *file)
{
	int	i;

	if (!file)
		return ;
	i = 0;
	while (file[i] != '\0')
		i++;
	if (file[i - 1] == '\n')
		file[i - 1] = '\0';
}

void	send_input_to_parent(t_mini *mini, char *input, t_rdr *rdr, char *total)
{
	input = ft_free(input);
	rdr->file = ft_free(rdr->file);
	if (!total)
		total = ft_strdup("");
	rdr->file = total;
	rdr->herepipe[0] = ft_close(rdr->herepipe[0], 3, NULL);
	fd_printf(rdr->herepipe[1], "%s", total);
	rdr->herepipe[1] = ft_close(rdr->herepipe[1], 3, NULL);
	ft_exit_shell(mini, 0, NULL, 2);
}

int	ft_pipe_heredoc(t_rdr *rdr, t_mini *mini, t_cmd *cmd)
{
	pipe(rdr->fdpipe);
	rdr->ret = 0;
	rdr->fork_id = fork();
	if (rdr->fork_id == 0)
	{
		rdr->fdpipe[0] = ft_close(rdr->fdpipe[0], 3, cmd);
		rdr->dup2_fd = dup2(rdr->fdpipe[1], STDOUT_FILENO);
		if (rdr->file)
			fd_printf(STDOUT_FILENO, "%s", rdr->file);
		rdr->fdpipe[1] = ft_close (rdr->fdpipe[1], 3, cmd);
		close_rdr_back(cmd);
		ft_exit_shell(mini, 0, NULL, 2);
	}
	else
	{
		rdr->fdpipe[1] = ft_close (rdr->fdpipe[1], 3, cmd);
		rdr->og_fd = dup(STDIN_FILENO);
		rdr->dup2_fd = dup2(rdr->fdpipe[0], STDIN_FILENO);
		waitpid(rdr->fork_id, &rdr->ret, 0);
		rdr->fdpipe[0] = ft_close (rdr->fdpipe[0], 3, cmd);
	}
	return (0);
}
