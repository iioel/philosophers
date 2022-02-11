/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 20:59:35 by ycornamu          #+#    #+#             */
/*   Updated: 2022/02/11 19:01:54 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>

static t_arg	**init_philos(pthread_t *philo, t_params *params)
{
	int				i;
	t_arg			**philo_args;
	t_arg			*arg;

	i = 0;
	philo = ft_calloc(params->nb_philo, sizeof(pthread_t));
	philo_args = ft_calloc(params->nb_philo, sizeof(t_arg *));
	if (! philo)
		return (NULL);
	while (i < params->nb_philo)
	{
		arg = ft_calloc(1, sizeof(t_arg));
		arg->id = i + 1;
		arg->alive = 1;
		//arg->malive = &(params->alive[i]);
		arg->malive = ft_calloc(1, sizeof(pthread_mutex_t));
		if (! arg->malive || pthread_mutex_init(arg->malive, NULL))
		{
			printf("error mutex failed\n");
			return (NULL);
		}
		arg->role = 0; //i % 3;
		arg->forkl = &(params->fork[i]);
		arg->forkr = &(params->fork[(i + 1) % params->nb_philo]);
		arg->mforkl = &(params->mfork[i]);
		arg->mforkr = &(params->mfork[(i + 1) % params->nb_philo]);
		arg->mlast_eat = ft_calloc(1, sizeof(pthread_mutex_t));
		if (! arg->mlast_eat || pthread_mutex_init(arg->mlast_eat, NULL))
		{
			printf("error mutex failed\n");
			return (NULL);
		}
		arg->params = params;
		printf("Philo %d created !\n", i + 1);
		if (pthread_create(&(philo[i]), NULL, run_philo, arg))
			return (NULL);
		philo_args[i] = arg;
		i++;
		
	}
	return (philo_args);
}

int	run_sim(pthread_t *philo, t_params *params)
{
	int	i;
	int	nb_alive;
	t_arg	**philo_args;

	nb_alive = params->nb_philo;
	philo_args = init_philos(philo, params);
	if (! philo_args)
		return (0);
	while (nb_alive)
	{
		i = 0;
		while (i < params->nb_philo)
		{
			pthread_mutex_lock(philo_args[i]->malive);
			pthread_mutex_lock(philo_args[i]->mlast_eat);
			//printf ("========== CHECK PHILO N %d ===============\n", i);
			if (philo_args[i]->alive && (get_time_mili() > (philo_args[i]->last_eat + params->t2die)))
			{
				philo_args[i]->alive = 0;
				philo_died(philo_args[i]);
				//pthread_detach(philo[i]);
				nb_alive--;
			}
			pthread_mutex_unlock(philo_args[i]->mlast_eat);
			pthread_mutex_unlock(philo_args[i]->malive);
			i++;
		}
		usleep(1000);
	}
	//if (arg->last_eat + arg->params->t2die < get_time_mili())
	//{
	//	printf("end: %ld\n", last_eat + arg->params->t2die);
	//	return (philo_died(arg));
	//}
	//pause();
//	pthread_join(philo[0], NULL);
//	pthread_join(philo[1], NULL);
//	pthread_join(philo[2], NULL);
	return (0);
}


