/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 14:21:04 by adkhalil          #+#    #+#             */
/*   Updated: 2026/09/05 20:29:54 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	dongle_is_busy(t_dongle *dongle, t_heap_node request, t_ms cooldown,
		t_sim *sim)
{
	int	s;

	pthread_mutex_lock(&dongle->mutex);
	s = dongle->in_use == 1;
	if (!s && dongle->released_time > 0)
		s = get_time_ms() - dongle->released_time < cooldown;
	if (!s)
		s = dongle->heap->nodes[0].coder_id != request.coder_id;
	if (sim_is_stopped(sim))
		s = 0;
	pthread_mutex_unlock(&dongle->mutex);
	return (s);
}

void	dongle_take(t_dongle *dongle, t_heap_node request, t_ms cooldown,
		t_sim *sim)
{
	pthread_mutex_lock(&dongle->mutex);
	heap_push(request, dongle->heap);
	pthread_mutex_unlock(&dongle->mutex);
	while (dongle_is_busy(dongle, request, cooldown, sim))
		usleep(500);
	pthread_mutex_lock(&dongle->mutex);
	heap_pop(dongle->heap);
	if (!sim_is_stopped(sim))
		dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->mutex);
}

void	dongle_release(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->in_use = 0;
	dongle->released_time = get_time_ms();
	pthread_mutex_unlock(&dongle->mutex);
}
