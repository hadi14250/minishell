/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 05:56:17 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/05 00:03:33 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Frees all allocated memory within the double pointer <split>
 * including itself.
 * 
 * @param split 
 */
void	*ft_free_split(char **split)
{
	int	i;

	i = -1;
	if (split)
		while (split[++i])
			split[i] = (char *) ft_free(split[i]);
	split = (char **) ft_free(split);
	return (NULL);
}
