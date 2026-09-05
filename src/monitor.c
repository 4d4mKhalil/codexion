/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 21:56:38 by adkhalil          #+#    #+#             */
/*   Updated: 2026/09/05 18:30:40 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_sim *sim)
{
	t_ms			last;
	unsigned int	i;

	i = 0;
	while (i < sim->cfg->number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].time_mutex);
		last = sim->coders[i].last_compile_time;
		pthread_mutex_unlock(&sim->coders[i].time_mutex);
		if (get_time_ms() - last > sim->cfg->time_to_burnout)
		{
			log_state(sim, sim->coders[i].id, "burned out");
			pthread_mutex_lock(&sim->stop_mutex);
			sim->stop = 1;
			pthread_mutex_unlock(&sim->stop_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (1)
	{
		if (sim_is_stopped(sim))
			return (NULL);
		if (check_burnout(sim))
			return (NULL);
		usleep(1000);
	}
}
