/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 14:21:04 by adkhalil          #+#    #+#             */
/*   Updated: 2026/09/02 17:15:01 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_take(t_dongle *dongle, t_heap_node request, t_ms cooldown)
{
	pthread_cond_t	cond;

	pthread_mutex_lock(&dongle->mutex);
	pthread_cond_init(&cond, NULL);
	request.cond = &cond;
	heap_push(request, &dongle->heap);
	while (dongle->in_use == 1 || (dongle->released_time > 0 && get_time_ms()
			- dongle->released_time < cooldown)
		|| dongle->heap.nodes[0].coder_id != request.coder_id)
	{
		pthread_cond_wait(&cond, &dongle->mutex);
	}
	heap_pop(&dongle->heap);
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->mutex);
	pthread_cond_destroy(&cond);
}

void	dongle_release(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->in_use = 0;
	dongle->released_time = get_time_ms();
	if (dongle->heap.size > 0)
		pthread_cond_signal(dongle->heap.nodes[0].cond);
	pthread_mutex_unlock(&dongle->mutex);
}
