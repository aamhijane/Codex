/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 22:36:31 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/24 00:39:42 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	allocate_dongles(t_sim *sim)
{
	size_t	size;

	size = sizeof(t_dongle) * sim->args->number_of_coders;
	sim->dongles = malloc(size);
	if (!sim->dongles)
		return (-1);
	memset(sim->dongles, 0, size);
	return (0);
}

int	allocate_coders(t_sim *sim)
{
	size_t	size;

	size = sizeof(t_coder) * sim->args->number_of_coders;
	sim->coders = malloc(size);
	if (!sim->coders)
		return (-1);
	memset(sim->coders, 0, size);
	return (0);
}

void	free_coders(t_sim *sim)
{
	if (sim->coders)
	{
		free(sim->coders);
		sim->coders = NULL;
	}
}

void	free_dongles(t_sim *sim)
{
	if (sim->dongles)
	{
		free(sim->dongles);
		sim->dongles = NULL;
	}
}
