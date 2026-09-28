/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 19:57:25 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/28 21:28:43 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	int		n_compiles_req;
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->sim->args->number_of_coders == 1)
		return (NULL);
	n_compiles_req = coder->sim->args->number_of_compiles_required;
	while (coder->compile_count < n_compiles_req)
	{
		take_dongle(coder->first);
		log_state(coder, IDLE);
		take_dongle(coder->second);
		log_state(coder, IDLE);
		coder->last_compile_time = get_time_in_ms();
		log_state(coder, COMPILING);
		usleep(coder->sim->args->time_to_compile * 1000);
		release_dongle(coder->first);
		release_dongle(coder->second);
		coder->compile_count += 1;
		log_state(coder, DEBUGGING);
		usleep(coder->sim->args->time_to_debug * 1000);
		log_state(coder, REFACTORING);
		usleep(coder->sim->args->time_to_refactor * 1000);
	}
	return (NULL);
}
