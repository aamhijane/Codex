/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 20:05:40 by ayamhija          #+#    #+#             */
/*   Updated: 2026/10/03 23:21:34 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_dongle(t_sim *sim, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	while (dongle->is_free == 0)
	{
		pthread_mutex_lock(&sim->sim_lock);
		if (sim->is_running == 0)
		{
			pthread_mutex_unlock(&sim->sim_lock);
			pthread_mutex_unlock(&dongle->lock);
			return (0);
		}
		pthread_mutex_unlock(&sim->sim_lock);
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
	dongle->is_free = 0;
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->is_free = 1;
	dongle->last_release_time = get_time_in_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

void	broadcast_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].lock);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].lock);
		i++;
	}
}
