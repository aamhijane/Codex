/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 19:57:25 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/27 19:43:58 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	log_state(coder, COMPILING);
	usleep(coder->sim->args->time_to_compile * 1000);
	log_state(coder, DEBUGGING);
	usleep(coder->sim->args->time_to_debug * 1000);
	log_state(coder, REFACTORING);
	usleep(coder->sim->args->time_to_refactor * 1000);
	return (NULL);
}
