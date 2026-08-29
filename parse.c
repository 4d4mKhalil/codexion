/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 11:25:03 by adkhalil          #+#    #+#             */
/*   Updated: 2026/08/29 22:24:39 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] <= '9' && str[i] >= '0'))
			return (0);
		i++;
	}
	return (1);
}

static int	is_valid_args(int argc, char *argv[])
{
	int	nbr;
	int	i;

	if (argc != 9)
		return (fprintf(stderr, "[Error]: Invalid number of arguments\n"), 0);
	i = 1;
	while (i < 8)
	{
		if (!is_number(argv[i]))
		{
			fprintf(stderr, "[Error]: Arg[%i]: '%s' must be integer\n", i + 1,
				argv[i]);
			return (0);
		}
		nbr = atoi(argv[i]);
		if (i == 1 && nbr < 1)
		{
			fprintf(stderr, "[Error]: Invalid arg, nbr_of_coders must > 0");
			return (0);
		}
		else if (nbr < 0)
			return (fprintf(stderr, "[Error]: Invalid argument{%i}\n", i), 0);
		i++;
	}
	return (1);
}

static int	is_valid_sched(char *arg)
{
	if (strcmp(arg, "fifo") != 0 && strcmp(arg, "edf") != 0)
	{
		fprintf(stderr, "[Error]: Invalid scheduling argument, must be");
		fprintf(stderr, " either 'fifo' or 'edf'\n");
		return (0);
	}
	return (1);
}

static int	args_checker(int argc, char *argv[])
{
	if (!is_valid_args(argc, argv))
		return (0);
	if (!is_valid_sched(argv[8]))
		return (0);
	return (1);
}

t_config	*cfg_fill(int argc, char **argv)
{
	t_config	*cfg;

	if (!args_checker(argc, argv))
		return (NULL);
	cfg = malloc(sizeof(t_config));
	if (!cfg)
		return (NULL);
	cfg->number_of_coders = atoi(argv[1]);
	cfg->time_to_burnout = atoi(argv[2]);
	cfg->time_to_compile = atoi(argv[3]);
	cfg->time_to_debug = atoi(argv[4]);
	cfg->time_to_refactor = atoi(argv[5]);
	cfg->number_of_compiles_required = atoi(argv[6]);
	cfg->dongle_cooldown = atoi(argv[7]);
	cfg->scheduler = argv[8];
	return (cfg);
}
