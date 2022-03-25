/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 10:01:38 by ycornamu          #+#    #+#             */
/*   Updated: 2022/03/19 15:24:14 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

void	cleanup_params(t_params *params);

int	cleanup(pthread_t **philo, t_params *params, t_arg **philo_args)
{
	int	i;

	i = 0;
	free(*philo);
	while (i < params->nb_philo)
	{
		free((philo_args)[i]->mlast_eat);
		free((philo_args)[i++]);
	}
	free(philo_args);
	cleanup_params(params);
	return (0);
}

void	cleanup_params(t_params *params)
{
	int	i;

	i = 0;
	free(params->malive);
	free(params->mprint);
	free(params->mfork);
	free(params->fork);
}
