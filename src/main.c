/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 21:51:16 by ayamhija          #+#    #+#             */
/*   Updated: 2026/10/02 23:56:57 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coder_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].lock);
		sim->coders[i].last_compile_time = sim->start_time;
		pthread_mutex_unlock(&sim->coders[i].lock);
		sim->coders[i].sim = sim;
		if (
			pthread_create(
				&sim->coders[i].thread,
				NULL,
				coder_routine,
				&sim->coders[i]) != 0
		)
			return (0);
		i++;
	}
	return (1);
}

static int	monitor_thread(t_sim *sim)
{
	if (
		pthread_create(
			&sim->monitor,
			NULL,
			monitor_routine,
			sim) != 0
	)
		return (0);
	return (1);
}

static int	start_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].lock);
		sim->coders[i].last_compile_time = sim->start_time;
		pthread_mutex_unlock(&sim->coders[i].lock);
		sim->coders[i].sim = sim;
		i++;
	}
	if (!coder_threads(sim))
		return (0);
	if (!monitor_thread(sim))
		return (0);
	i = 0;
	while (i < sim->args->number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
	return (1);
}

int	main(int argc, char **argv)
{
	t_args	args;
	t_sim	sim;

	if (prepare_args(argc, argv, &args) != 0)
		return (1);
	if (init_simulation(&sim, &args) != 0)
	{
		free(args.scheduler);
		return (1);
	}
	pthread_mutex_lock(&sim.sim_lock);
	sim.start_time = get_time_in_ms();
	sim.is_running = 1;
	pthread_mutex_unlock(&sim.sim_lock);
	if (!start_threads(&sim))
	{
		fprintf(stderr, "ERROR: threads fails due creation");
		cleanup_simulation(&sim);
		return (1);
	}
	cleanup_simulation(&sim);
	return (0);
}
