/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 19:57:25 by ayamhija          #+#    #+#             */
/*   Updated: 2026/10/02 00:15:56 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	coders_compiles_count(t_sim *sim, int idx)
{
	int	count;
	int	n_required;

	n_required = sim->args->number_of_compiles_required;
	count = 0;
	pthread_mutex_lock(&sim->coders[idx].lock);
	if (sim->coders[idx].compile_count == n_required)
		count++;
	pthread_mutex_unlock(&sim->coders[idx].lock);
	return (count);
}

static void	coder_compiling(t_coder *coder, int *count)
{
	pthread_mutex_lock(&coder->lock);
	coder->last_compile_time = get_time_in_ms();
	pthread_mutex_unlock(&coder->lock);
	log_state(coder, COMPILING);
	usleep(coder->sim->args->time_to_compile * 1000);
	release_dongle(coder->first);
	release_dongle(coder->second);
	pthread_mutex_lock(&coder->lock);
	coder->compile_count += 1;
	*count = coder->compile_count;
	pthread_mutex_unlock(&coder->lock);
}

static void	coder_debug_refactor(t_coder *coder)
{
	log_state(coder, DEBUGGING);
	usleep(coder->sim->args->time_to_debug * 1000);
	log_state(coder, REFACTORING);
	usleep(coder->sim->args->time_to_refactor * 1000);
}

static int	coder_routine_core(t_coder *coder, int *count)
{
	if (is_sim_over(coder->sim))
		return (0);
	if (take_dongle(coder->sim, coder, coder->first) == 0)
		return (0);
	log_state(coder, IDLE);
	if (is_sim_over(coder->sim))
		return (release_dongle(coder->first), 0);
	if (take_dongle(coder->sim, coder, coder->second) == 0)
		return (release_dongle(coder->first), 0);
	log_state(coder, IDLE);
	coder_compiling(coder, count);
	if (is_sim_over(coder->sim))
		return (0);
	coder_debug_refactor(coder);
	return (1);
}

void	*coder_routine(void *arg)
{
	int		compile_count;
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->sim->args->number_of_coders == 1)
		return (NULL);
	pthread_mutex_lock(&coder->lock);
	compile_count = coder->compile_count;
	pthread_mutex_unlock(&coder->lock);
	while (compile_count < coder->sim->args->number_of_compiles_required)
	{
		if (!coder_routine_core(coder, &compile_count))
			return (NULL);
	}
	return (NULL);
}
