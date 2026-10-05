/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 19:37:09 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/28 21:14:52 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_coder *coder, enum e_coder_state state)
{
	int		is_running;
	long	timestamp_ms;
	long	current_time;

	current_time = get_time_in_ms();
	timestamp_ms = current_time - coder->sim->start_time;
	pthread_mutex_lock(&coder->sim->sim_lock);
	is_running = coder->sim->is_running;
	pthread_mutex_unlock(&coder->sim->sim_lock);
	pthread_mutex_lock(&coder->sim->log_lock);
	if (is_running == 1)
	{
		if (state == IDLE)
			printf("%ld %d has taken a dongle\n", timestamp_ms, coder->id);
		else if (state == COMPILING)
			printf("%ld %d is compiling\n", timestamp_ms, coder->id);
		else if (state == DEBUGGING)
			printf("%ld %d is debugging\n", timestamp_ms, coder->id);
		else if (state == REFACTORING)
			printf("%ld %d is refactoring\n", timestamp_ms, coder->id);
		else if (state == BURNOUT)
			printf("%ld %d burned out\n", timestamp_ms, coder->id);
	}
	pthread_mutex_unlock(&coder->sim->log_lock);
}
