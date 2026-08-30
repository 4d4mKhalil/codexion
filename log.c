/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 22:58:09 by adkhalil          #+#    #+#             */
/*   Updated: 2026/08/30 01:13:32 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_sim *sim, int coder_id, char *state)
{
	t_ms	elapsed_ms;

	pthread_mutex_lock(&sim->log_mutex);
	elapsed_ms = get_time_ms() - sim->start_time;
	printf("%lu %u %s\n", elapsed_ms, coder_id, state);
	pthread_mutex_unlock(&sim->log_mutex);
}
