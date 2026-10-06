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

static void	set_key(t_sim *sim, t_coder *coder, long *key, long *seq)
{
	if (strcmp(sim->args->scheduler, "fifo") == 0)
		*key = get_time_in_ms();
	else
	{
		pthread_mutex_lock(&coder->lock);
		*key = coder->last_compile_time + sim->args->time_to_burnout;
		pthread_mutex_unlock(&coder->lock);
	}
	pthread_mutex_lock(&sim->sim_lock);
	*seq = sim->next_seq++;
	pthread_mutex_unlock(&sim->sim_lock);
}

static int	wait_for_dongle(t_sim *sim, t_dongle *dongle, t_coder *coder)
{
	long		cooldown_end;
	long		release_time;
	long		remaining_ms;

	if (dongle->is_free == 0
		|| heap_peek(&dongle->queue).coder_id != coder->id)
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	else
	{
		release_time = dongle->last_release_time;
		cooldown_end = release_time + sim->args->dongle_cooldown;
		if (get_time_in_ms() >= cooldown_end)
			return (1);
		else
		{
			remaining_ms = cooldown_end - get_time_in_ms();
			pthread_mutex_unlock(&dongle->lock);
			usleep(remaining_ms * 1000);
			pthread_mutex_lock(&dongle->lock);
		}
	}
	return (0);
}

int	take_dongle(t_sim *sim, t_coder *coder, t_dongle *dongle)
{
	long		key;
	long		seq;
	t_heap_node	node;

	set_key(sim, coder, &key, &seq);
	pthread_mutex_lock(&dongle->lock);
	node.coder_id = coder->id;
	node.key = key;
	node.seq = seq;
	heap_push(&dongle->queue, node);
	while (1)
	{
		if (is_sim_over(sim))
		{
			heap_remove(&dongle->queue, coder->id);
			pthread_mutex_unlock(&dongle->lock);
			return (0);
		}
		if (wait_for_dongle(sim, dongle, coder))
			break ;
	}
	heap_pop(&dongle->queue);
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
