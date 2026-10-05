/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 21:51:16 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/27 19:44:23 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	start_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		sim->coders[i].sim = sim;
		if (
			pthread_create(
				&sim->coders[i].thread,
				NULL,
				coder_routine,
				&sim->coders[i]) != 0
		)
			return (-1);
		i++;
	}
	i = 0;
	while (i < sim->args->number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	return (0);
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
	if (start_threads(&sim) != 0)
	{
		printf("ERROR: threads fails due creation");
		cleanup_simulation(&sim);
		return (1);
	}
	cleanup_simulation(&sim);
	return (0);
}
