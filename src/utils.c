/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 21:51:04 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/27 18:28:50 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_valid_number(char *arg)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = strlen(arg);
	while (i < len)
	{
		if (!isdigit(arg[i]))
			return (-1);
		i++;
	}
	return (0);
}

int	is_num_overflow(char *arg)
{
	size_t	len;
	int		result;

	len = strlen(arg);
	result = 0;
	if (len > 10)
		return (0);
	if (len == 10)
	{
		result = atoi(arg);
		if (result < 0)
			return (0);
	}
	return (-1);
}

char	*ft_trim(char *str)
{
	char	*end;

	while (isspace((unsigned char)*str))
		str++;
	if (*str == 0)
		return (str);
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end))
		end--;
	end[1] = '\0';
	return (str);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

long	get_time_in_ms(void)
{
	uint64_t		start_in_ms;
	uint64_t		time_in_ms;
	struct timespec	start;

	clock_gettime(CLOCK_MONOTONIC, &start);
	start_in_ms = ((uint64_t)start.tv_sec * 1000);
	time_in_ms = start_in_ms + ((uint64_t)start.tv_nsec / 1000000);
	return (time_in_ms);
}
