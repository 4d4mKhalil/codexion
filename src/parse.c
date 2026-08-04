/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 11:25:03 by adkhalil          #+#    #+#             */
/*   Updated: 2026/08/03 12:02:13 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static int      is_number(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (!(str[i] >= '0' && str[i] <= '9'))
            return (ERROR);
        i++;
    }
    return (SUCCESS);
}

static int     validate_arguments(int argc, char **argv)
{
    if (argc != 9)
        return (ERROR);
    
}
int     parse_arguments(t_config *config, int argc, char **argv)
{
    if (validate_arguments(argc, argv) == ERROR)
        return (ERROR);
}