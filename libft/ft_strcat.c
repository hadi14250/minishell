/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 06:33:12 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/05 20:47:33 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief append a copy of the null-terminated string <src> to the end of the
 * null-terminated string <dst>, then add a terminating `\0'.
 * 
 * @param dst 
 * @param src 
 * @return a pointer to <dst>
 */
char	*ft_strcat(char *dst, const char *src)
{
	size_t	i;
	size_t	d;

	i = 0;
	d = ft_strlen(dst);
	while (src[i])
	{
		dst[d + i] = src[i];
		i++;
	}
	dst[d + i] = 0;
	return (dst);
}
