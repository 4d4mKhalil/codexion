/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 22:41:39 by adkhalil          #+#    #+#             */
/*   Updated: 2026/08/29 22:56:06 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_ms	get_time_ms(void)
{
	t_ms			ts;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	ts = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (ts);
}

void	precise_sleep(t_ms duration)
{
	usleep(duration * 1000);
}
