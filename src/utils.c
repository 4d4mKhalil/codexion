/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 22:50:16 by adkhalil          #+#    #+#             */
/*   Updated: 2026/09/05 18:42:36 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_is_stopped(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->stop_mutex);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stop);
}

void	cleanup(t_sim *sim)
{
	unsigned int	i;

	i = 0;
	while (i < sim->cfg->number_of_coders)
	{
		free(sim->dongles[i].heap->nodes);
		free(sim->dongles[i].heap);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_mutex_destroy(&sim->coders[i].time_mutex);
		i++;
	}
	free(sim->dongles);
	free(sim->coders);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	free(sim->cfg);
	free(sim);
}

int	check_completion(t_coder *coder, t_sim *sim)
{
	unsigned int	i;
	unsigned int	done;

	pthread_mutex_lock(&sim->stop_mutex);
	coder->compile_count++;
	i = 0;
	done = 0;
	while (i < sim->cfg->number_of_coders)
	{
		if (sim->coders[i].compile_count
			>= sim->cfg->number_of_compiles_required)
			done++;
		i++;
	}
	if (done == sim->cfg->number_of_coders)
		sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (sim->stop);
}
