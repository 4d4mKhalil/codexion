/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 11:07:26 by adkhalil          #+#    #+#             */
/*   Updated: 2026/08/03 11:54:47 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# define SUCCESS 0
# define ERROR   1

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_simulation t_simulation;

typedef struct s_config
{
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_sleep;
    int must_compile;
} t_config;

typedef struct s_dongle
{
    int id;
    pthread_mutex_t mutex;
} t_dongle;

typedef struct s_coder
{
    int                 id;
    pthread_t           thread;

    t_dongle            *left;
    t_dongle            *right;

    long                last_compile;

    int                 compile_count;

    t_simulation *sim;
} t_coder;

typedef struct s_simulation
{
    t_config        config;

    t_coder         *coders;
    t_dongle        *dongles;

    long            start_time;

    int             stop_simulation;

    pthread_mutex_t print_mutex;
    pthread_mutex_t stop_mutex;

} t_simulation;

int     parse_arguments(t_config *config, int argc, char **argv);
static int     validate_arguments(int argc, char **argv);
static int     is_number(char *str);
void    print_error(char *msg);

#endif