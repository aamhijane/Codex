/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 20:05:40 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/28 21:12:23 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	while (dongle->is_free == 0)
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	dongle->is_free = 0;
	pthread_mutex_unlock(&dongle->lock);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->is_free = 1;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}
