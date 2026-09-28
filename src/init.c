/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 22:46:34 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/28 21:32:14 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongle(t_sim *sim, int i)
{
	if (pthread_mutex_init(&sim->dongles[i].lock, NULL) != 0)
		return (-1);
	if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
		return (-1);
	sim->dongles[i].is_free = 1;
	sim->dongles[i].last_release_time = 0;
	return (0);
}

static int	init_coder(t_sim *sim, int i)
{
	int	n;

	n = sim->args->number_of_coders;
	if (pthread_mutex_init(&sim->coders[i].lock, NULL) != 0)
		return (-1);
	sim->coders[i].id = i + 1;
	sim->coders[i].first = &sim->dongles[i];
	sim->coders[i].second = &sim->dongles[(i + 1) % n];
	if (n > 1 && i == n - 1)
	{
		sim->coders[i].first = &sim->dongles[0];
		sim->coders[i].second = &sim->dongles[i];
	}
	if (n == 1)
		sim->coders[i].second = NULL;
	return (0);
}

int	init_simulation(t_sim *sim, t_args *args)
{
	int		i;

	if (pthread_mutex_init(&sim->sim_lock, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&sim->log_lock, NULL) != 0)
		return (-1);
	sim->args = args;
	allocate_dongles(sim);
	allocate_coders(sim);
	i = 0;
	while (i < sim->args->number_of_coders)
	{
		if (init_dongle(sim, i) != 0)
			return (-1);
		if (init_coder(sim, i) != 0)
			return (-1);
		i++;
	}
	return (0);
}

void	cleanup_simulation(t_sim *sim)
{
	int		i;

	pthread_mutex_destroy(&sim->sim_lock);
	pthread_mutex_destroy(&sim->log_lock);
	i = 0;
	while (i < sim->args->number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->coders[i].lock);
		i++;
	}
	free_dongles(sim);
	free_coders(sim);
	free(sim->args->scheduler);
}
