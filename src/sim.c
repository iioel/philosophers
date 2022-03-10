/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 20:59:35 by ycornamu          #+#    #+#             */
/*   Updated: 2022/03/10 21:01:44 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>

int	exit_mutex(void)
{
	printf("error mutex failed\n");
	return (1);
}

static int	create_philo(pthread_t *philo, t_params *params, int i,
		t_arg ***philo_args)
{
	t_arg			*arg;

	arg = ft_calloc(1, sizeof(t_arg));
	arg->id = i + 1;
	arg->alive = 0;
	arg->malive = ft_calloc(1, sizeof(pthread_mutex_t));
	if (! arg->malive || pthread_mutex_init(arg->malive, NULL))
		return (exit_mutex());
	arg->role = 0;
	arg->forkl = &(params->fork[i]);
	arg->forkr = &(params->fork[(i + 1) % params->nb_philo]);
	arg->mforkl = &(params->mfork[i]);
	arg->mforkr = &(params->mfork[(i + 1) % params->nb_philo]);
	arg->mlast_eat = ft_calloc(1, sizeof(pthread_mutex_t));
	if (! arg->mlast_eat || pthread_mutex_init(arg->mlast_eat, NULL))
		return (exit_mutex());
	arg->params = params;
	if (pthread_create(&(philo[i]), NULL, run_philo, arg))
		return (1);
	(*philo_args)[i] = arg;
	return (0);
}

static t_arg	**init_philos(pthread_t *philo, t_params *params)
{
	int		i;
	t_arg	**philo_args;

	i = 0;
	philo = ft_calloc(params->nb_philo, sizeof(pthread_t));
	philo_args = ft_calloc(params->nb_philo, sizeof(t_arg *));
	//if (! philo || ! philo_args)
	if (! philo)
		return (NULL);
	params->start_time = get_time_mili();
	while (i < params->nb_philo)
	{
		if (! (i % 2))
			if (create_philo(philo, params, i, &philo_args))
				return (NULL);
		i++;
	}
	usleep(params->t2eat * 1000);
	i = 0;
	while (i < params->nb_philo)
	{
		if (i % 2)
			if (create_philo(philo, params, i, &philo_args))
				return (NULL);
		i++;
	}
	return (philo_args);
}

int	run_sim(pthread_t *philo, t_params *params)
{
	int		i;
	int		end;
	int		nb_alive;
	t_arg	**philo_args;

	end = 0;
	nb_alive = params->nb_philo;
	philo_args = init_philos(philo, params);
	if (! philo_args)
		return (0);
	while (nb_alive && ! end)
	{
		i = 0;
		nb_alive = 0;
		while (i < params->nb_philo && ! end)
		{
			pthread_mutex_lock(philo_args[i]->malive);
			pthread_mutex_lock(philo_args[i]->mlast_eat);
			if (philo_args[i]->alive && (get_time_mili()
					> (philo_args[i]->last_eat + params->t2die)))
			{
				philo_args[i]->alive = 0;
				philo_died(philo_args[i]);
				end = 1;
			}
			nb_alive += philo_args[i]->alive;
			pthread_mutex_unlock(philo_args[i]->mlast_eat);
			pthread_mutex_unlock(philo_args[i]->malive);
			i++;
		}
		usleep(1000);
	}
	i = 0;
	while (i < params->nb_philo)
	{
		pthread_mutex_lock(philo_args[i]->malive);
		philo_args[i]->alive = 0;
		pthread_mutex_unlock(philo_args[i]->malive);
		i++;
	}
	return (0);
}
