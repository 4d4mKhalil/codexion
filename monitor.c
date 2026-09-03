/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 21:56:38 by adkhalil          #+#    #+#             */
/*   Updated: 2026/09/02 22:42:12 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routine(void *arg)
{
	t_sim			*sim;
	unsigned int	i;

	sim = (t_sim *)arg;
	while (1)
	{
		if (sim_is_stopped(sim))
			return (NULL);
		i = 0;
		while (i < sim->cfg->number_of_coders)
		{
			if (get_time_ms()
				- sim->coders[i].last_compile_time > sim->cfg->time_to_burnout)
			{
				log_state(sim, sim->coders[i].id, "burned out");
				pthread_mutex_lock(&sim->stop_mutex);
				sim->stop = 1;
				pthread_mutex_unlock(&sim->stop_mutex);
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
}
