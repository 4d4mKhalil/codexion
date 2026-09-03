/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 23:53:09 by adkhalil          #+#    #+#             */
/*   Updated: 2026/09/03 00:11:10 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config		*cfg;
	t_sim			*sim;
	unsigned int	i;

	cfg = cfg_fill(argc, argv);
	if (!cfg)
		return (1);
	sim = sim_init(cfg);
	if (!sim)
		return (free(cfg), 1);
	dongles_init(sim);
	sim->start_time = get_time_ms();
	coders_init(sim);
	threads_start(sim);
	i = 0;
	while (i < sim->cfg->number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
	return (0);
}
