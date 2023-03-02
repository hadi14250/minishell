/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_srcs2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 23:30:00 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/04 23:32:45 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	empty_file_check(t_rdr *rdr)
{
	if (!rdr->file[0])
	{
		fd_printf(2, "minishell: : No such file or directory\n");
		g_exit_code = 1;
		return (true);
	}
	return (false);
}

int	parse_redirect_errors(t_mini *mini, t_cmd *cmd, int flag)
{
	if (flag == 3)
		return (error_set_print_close(mini, cmd, 3));
	else if (flag == 2)
		return (error_set_print_close(mini, cmd, 2));
	else if (flag == 1)
		return (error_set_print_close(mini, cmd, 1));
	else
		return (parse_redirect(mini, cmd));
}

int	error_set_print_close(t_mini *mini, t_cmd *cmd, int error)
{
	if (error == 2)
	{
		g_exit_code = 1;
		close_rdr_files(mini, cmd);
		return (1);
	}
	if (error == 3)
	{
		g_exit_code = 1;
		close_rdr_files(mini, cmd);
		return (1);
	}
	g_exit_code = error;
	fd_printf (2, "minishell: %s\n", strerror(errno));
	close_rdr_files(mini, cmd);
	return (1);
}
