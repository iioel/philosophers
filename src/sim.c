/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 20:59:35 by ycornamu          #+#    #+#             */
/*   Updated: 2021/12/15 01:14:51 by yoel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static int	init_philos(pthread_t *philo, t_params *params)
{
	int				i;
	t_arg			*arg;
	//pthread_mutex_t	*alive;

	i = 0;
	philo = ft_calloc(params->nb_philo, sizeof(pthread_t));
	//alive = ft_calloc(params.nb_philo, sizeof(pthread_mutex_t *));
	if (! philo)
		return (1);
	while (i < params->nb_philo)
	{
		arg = ft_calloc(1, sizeof(t_arg));
		arg->id = i + 1;
	//	arg->alive = 
	//	pthread_mutex_init(&(arg->alive));
		arg->role = 0; //i % 3;
		arg->forkl = &(params->fork[i]);
		arg->forkr = &(params->fork[(i + 1) % params->nb_philo]);
		arg->mforkl = &(params->mfork[i]);
		arg->mforkr = &(params->mfork[(i + 1) % params->nb_philo]);
		arg->params = params;
		printf("Philo %d created !\n", i + 1);
		if (pthread_create(&(philo[i]), NULL, run_philo, arg))
			return (1);
		i++;
		
	}
	return (0);
}

int	run_sim(pthread_t *philo, t_params *params)
{
	init_philos(philo, params);
	pause();
	return (0);
}


