/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 17:18:46 by adkhalil          #+#    #+#             */
/*   Updated: 2026/09/05 17:12:44 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_heap_node	demand_request(t_coder *coder, t_sim *sim)
{
	t_heap_node	request;

	request.coder_id = coder->id;
	request.cond = NULL;
	if (strcmp(sim->cfg->scheduler, "fifo") == 0)
		request.priority = get_time_ms();
	else
		request.priority = coder->last_compile_time + sim->cfg->time_to_burnout;
	return (request);
}

static int	check_completion(t_coder *coder, t_sim *sim)
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

static int	take_dongles(t_coder *coder, t_sim *sim)
{
	t_heap_node	request;

	request = demand_request(coder, sim);
	dongle_take(coder->left, request, sim->cfg->dongle_cooldown, sim);
	if (sim_is_stopped(sim))
	{
		dongle_release(coder->left);
		return (0);
	}
	log_state(sim, coder->id, "has taken a dongle");
	request = demand_request(coder, sim);
	dongle_take(coder->right, request, sim->cfg->dongle_cooldown, sim);
	if (sim_is_stopped(sim))
	{
		dongle_release(coder->left);
		dongle_release(coder->right);
		return (0);
	}
	log_state(sim, coder->id, "has taken a dongle");
	return (1);
}

static void	compile_phase(t_coder *coder, t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	if (coder->compile_count >= sim->cfg->number_of_compiles_required)
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&sim->stop_mutex);
	if (!take_dongles(coder, sim))
		return ;
	pthread_mutex_lock(&coder->time_mutex);
	coder->last_compile_time = get_time_ms();
	pthread_mutex_unlock(&coder->time_mutex);
	log_state(sim, coder->id, "is compiling");
	precise_sleep(sim->cfg->time_to_compile);
	if (check_completion(coder, sim))
	{
		dongle_release(coder->left);
		dongle_release(coder->right);
		return ;
	}
	dongle_release(coder->left);
	dongle_release(coder->right);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (1)
	{
		if (sim_is_stopped(sim))
			return (NULL);
		compile_phase(coder, sim);
		if (sim_is_stopped(sim))
			return (NULL);
        if (coder->compile_count >= sim->cfg->number_of_compiles_required)
        {
            pthread_mutex_lock(&coder->time_mutex);
            coder->last_compile_time = get_time_ms();
            pthread_mutex_unlock(&coder->time_mutex);
            usleep(10000);
            continue ;
        }
		log_state(sim, coder->id, "is debugging");
		precise_sleep(sim->cfg->time_to_debug);
		if (sim_is_stopped(sim))
			return (NULL);
		log_state(sim, coder->id, "is refactoring");
		precise_sleep(sim->cfg->time_to_refactor);
	}
}
