/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 18:19:10 by ayamhija          #+#    #+#             */
/*   Updated: 2026/10/01 23:56:15 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_sim_over(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_lock);
	if (sim->is_running == 0)
	{
		pthread_mutex_unlock(&sim->sim_lock);
		return (1);
	}
	pthread_mutex_unlock(&sim->sim_lock);
	return (0);
}

static void	stop_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_lock);
	sim->is_running = 0;
	pthread_mutex_unlock(&sim->sim_lock);
}

static int	burnout_detected(t_sim *sim, int coder_idx)
{
	long	last_compile_t;

	pthread_mutex_lock(&sim->coders[coder_idx].lock);
	last_compile_t = sim->coders[coder_idx].last_compile_time;
	pthread_mutex_unlock(&sim->coders[coder_idx].lock);
	if ((get_time_in_ms() - last_compile_t) >= sim->args->time_to_burnout)
	{
		log_state(&sim->coders[coder_idx], BURNOUT);
		stop_simulation(sim);
		broadcast_all(sim);
		return (1);
	}
	return (0);
}

static int	is_coders_finished(t_sim *sim, int n_finished)
{
	if (n_finished == sim->args->number_of_coders)
	{
		stop_simulation(sim);
		broadcast_all(sim);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	int		i;
	int		finished;
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!is_sim_over(sim))
	{
		i = 0;
		finished = 0;
		while (i < sim->args->number_of_coders)
		{
			if (burnout_detected(sim, i))
				return (NULL);
			finished = coders_compiles_count(sim, i);
			i++;
		}
		if (is_coders_finished(sim, finished))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
