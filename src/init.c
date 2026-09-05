/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 22:52:02 by adkhalil          #+#    #+#             */
/*   Updated: 2026/09/05 18:42:29 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_sim	*sim_init(t_config *cfg)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (NULL);
	sim->cfg = cfg;
	sim->stop = 0;
	pthread_mutex_init(&sim->log_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
	return (sim);
}

void	dongles_init(t_sim *sim)
{
	unsigned int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->cfg->number_of_coders);
	if (!sim->dongles)
		return ;
	i = 0;
	while (i < sim->cfg->number_of_coders)
	{
		sim->dongles[i].heap = heap_init(sim->cfg->number_of_coders);
		if (!sim->dongles[i].heap)
			return ;
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		sim->dongles[i].in_use = 0;
		sim->dongles[i].released_time = 0;
		sim->dongles[i].id = i;
		i++;
	}
}

static void	assign_dongles(t_sim *sim, unsigned int i)
{
	unsigned int	n;

	n = sim->cfg->number_of_coders;
	if (i == n - 1)
	{
		sim->coders[i].left = &sim->dongles[(i + 1) % n];
		sim->coders[i].right = &sim->dongles[i];
	}
	else
	{
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[(i + 1) % n];
	}
}

void	coders_init(t_sim *sim)
{
	unsigned int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->cfg->number_of_coders);
	if (!sim->coders)
		return ;
	i = 0;
	while (i < sim->cfg->number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_time = sim->start_time;
		sim->coders[i].sim = sim;
		assign_dongles(sim, i);
		pthread_mutex_init(&sim->coders[i].time_mutex, NULL);
		i++;
	}
}

void	threads_start(t_sim *sim)
{
	unsigned int	i;

	i = 0;
	while (i < sim->cfg->number_of_coders)
	{
		pthread_create(&sim->coders[i].thread, NULL, coder_routine,
			&sim->coders[i]);
		i++;
	}
	pthread_create(&sim->monitor, NULL, monitor_routine, sim);
}
