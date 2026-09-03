/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 11:07:26 by adkhalil          #+#    #+#             */
/*   Updated: 2026/09/03 01:46:02 by adkhalil         ###   ########.fr       */
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
	t_heap				*heap;
	pthread_mutex_t		mutex;
	t_ms				released_time;
	int					in_use;
	int					id;
}						t_dongle;

typedef struct s_sim	t_sim;

typedef struct s_coder
{
	unsigned int		id;
	pthread_t			thread;
	t_dongle			*left;
	t_dongle			*right;
	unsigned int		compile_count;
	t_ms				last_compile_time;
	t_sim				*sim;
}						t_coder;

typedef struct s_sim
{
	t_config			*cfg;
	t_coder				*coders;
	t_dongle			*dongles;
	t_ms				start_time;
	pthread_mutex_t		log_mutex;
	int					stop;
	pthread_mutex_t		stop_mutex;
	pthread_t			monitor;
}						t_sim;

struct timespec	ms_to_timespec(t_ms ms)
{
	struct timespec	ts;

	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_nsec += (ms * 1000000);
	ts.tv_sec += ts.tv_nsec / 1000000000;
	ts.tv_nsec %= 1000000000;
	return (ts);
}

t_heap					*heap_init(int capacity);
t_heap_node				heap_pop(t_heap *heap);
t_config				*cfg_fill(int argc, char **argv);
t_ms					get_time_ms(void);
void					heap_push(t_heap_node node, t_heap *heap);
void					precise_sleep(t_ms duration);
void					dongle_take(t_dongle *dongle, t_heap_node request,
							t_ms cooldown, t_sim *sim);
void					dongle_release(t_dongle *dongle);
void					log_state(t_sim *sim, int coder_id, char *state);
int						sim_is_stopped(t_sim *sim);
void					*coder_routine(void *arg);
void					*monitor_routine(void *arg);
t_sim					*sim_init(t_config *cfg);
void					dongles_init(t_sim *sim);
void					coders_init(t_sim *sim);
void					threads_start(t_sim *sim);

#endif