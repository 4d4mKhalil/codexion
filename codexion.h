/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 11:07:26 by adkhalil          #+#    #+#             */
/*   Updated: 2026/08/30 02:02:50 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef unsigned long	t_ms;

typedef struct s_heap_node
{
	unsigned int		coder_id;
	t_ms				priority;
	pthread_cond_t		*cond;
}						t_heap_node;

typedef struct s_heap
{
	t_heap_node			*nodes;
	int					size;
	int					capacity;
}						t_heap;

typedef struct s_dongle
{
	t_heap				queue;
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	t_ms				released_time;
	int					in_use;
	int					id;
}						t_dongle;

typedef struct s_sim
{
	t_ms				start_time;
	pthread_mutex_t		log_mutex;
}						t_sim;

typedef struct s_config
{
	unsigned int		number_of_coders;
	t_ms				time_to_burnout;
	t_ms				time_to_compile;
	t_ms				time_to_debug;
	t_ms				time_to_refactor;
	unsigned int		number_of_compiles_required;
	t_ms				dongle_cooldown;
	char				*scheduler;
}						t_config;

t_heap					*heap_init(int capacity);
t_heap_node				heap_pop(t_heap *heap);
t_config				*cfg_fill(int argc, char **argv);
t_ms					get_time_ms(void);
void					heap_push(t_heap_node node, t_heap *heap);
void					precise_sleep(t_ms duration);

#endif