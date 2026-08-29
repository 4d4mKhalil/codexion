/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 11:07:26 by adkhalil          #+#    #+#             */
/*   Updated: 2026/08/29 22:25:56 by adkhalil         ###   ########.fr       */
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

t_config	*cfg_fill(int argc, char **argv);

#endif