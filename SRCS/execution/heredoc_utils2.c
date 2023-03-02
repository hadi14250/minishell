/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 21:04:08 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/06 01:32:38 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	read_heredoc_child(t_rdr *rdr)
{
	int		i;
	char	*buff;
	char	*str;
	char	*tmp;

	str = NULL;
	tmp = NULL;
	buff = ft_calloc(sizeof(char *), 2);
	i = 42;
	while (i)
	{
		i = read(rdr->herepipe[0], buff, 1);
		tmp = str;
		if (!buff[0])
			break ;
		str = gl_strjoin(tmp, buff);
		free(tmp);
	}
	rdr->file = ft_free(rdr->file);
	trim_last_new_line(str);
	rdr->file = str;
	buff = ft_free(buff);
}

int	ft_fork_heredoc(t_rdr *rdr)
{
	rdr->fork_id = fork();
	if (rdr->fork_id == -1)
	{
		g_exit_code = errno;
		fd_printf(2, "minishell: fork: %s\n", strerror(errno));
		return (-1);
	}
	return (0);
}

int	pipe_heredoc(t_rdr *rdr)
{
	int	error;

	error = pipe(rdr->herepipe);
	if (error == -1)
	{
		g_exit_code = errno;
		fd_printf(2, "minishell: pipe: %s\n", strerror(errno));
		return (-1);
	}
	return (0);
}

int	here_check_exit(int status, t_rdr *rdr)
{
	if (WEXITSTATUS(status) == 69)
	{
		ft_close(rdr->herepipe[1], 3, NULL);
		ft_close(rdr->herepipe[0], 3, NULL);
		g_exit_code = 1;
		return (69);
	}
	return (0);
}

int	take_input_from_child(t_mini *mini, t_rdr *rdr)
{
	int	status;

	status = 0;
	if (pipe_heredoc(rdr) == -1)
		return (-1);
	if (ft_fork_heredoc(rdr) == -1)
		return (-1);
	if (rdr->fork_id == 0)
		take_heredoc_input(rdr, mini);
	else
	{
		waitpid(rdr->fork_id, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 69)
			return (here_check_exit(status, rdr));
		ft_close(rdr->herepipe[1], 3, NULL);
		read_heredoc_child(rdr);
		ft_close(rdr->herepipe[0], 3, NULL);
	}
	signal(SIGINT, &ft_interupt);
	return (0);
}
