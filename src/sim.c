/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 20:59:35 by ycornamu          #+#    #+#             */
/*   Updated: 2022/03/25 16:56:24 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>

static int	create_philo(pthread_t **philo, t_params *params, int i,
		t_arg ***philo_args)
{
	t_arg			*arg;

	arg = ft_calloc(1, sizeof(t_arg));
	if (! arg)
		return (1);
	arg->id = i + 1;
	arg->alive = 0;
	arg->forkl = &(params->fork[i]);
	arg->forkr = &(params->fork[(i + 1) % params->nb_philo]);
	arg->mforkl = &(params->mfork[i]);
	arg->mforkr = &(params->mfork[(i + 1) % params->nb_philo]);
	arg->mlast_eat = ft_calloc(1, sizeof(pthread_mutex_t));
	if (! arg->mlast_eat || pthread_mutex_init(arg->mlast_eat, NULL))
		return (exit_mutex());
	arg->params = params;
	if (pthread_create(&(*philo)[i], NULL, run_philo, arg))
		return (exit_thread());
	(*philo_args)[i] = arg;
	return (0);
}

static t_arg	**init_philos(pthread_t **philo, t_params *params)
{
	int		i;
	t_arg	**philo_args;

	i = 0;
	*philo = ft_calloc(params->nb_philo, sizeof(pthread_t));
	philo_args = ft_calloc(params->nb_philo, sizeof(t_arg *));
	if (! *philo || ! philo_args)
		return (NULL);
	params->start_time = get_time_mili();
	while (i < params->nb_philo)
	{
		if (create_philo(philo, params, i, &philo_args))
			return (NULL);
		i++;
	}
	usleep(params->t2die / 2 * 1000);
	return (philo_args);
}

int	check_philo_exceed_time(t_arg **philo_args, t_params *params, int *nb_alive)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (i < params->nb_philo && ! res)
	{
		pthread_mutex_lock(params->malive);
		pthread_mutex_lock(philo_args[i]->mlast_eat);
		if (params->alive && (get_time_mili()
				> (philo_args[i]->last_eat + params->t2die))
			&& philo_args[i]->nb_eat != params->nb2eat && philo_args[i]->alive)
		{
			pthread_mutex_unlock(params->malive);
			philo_died(philo_args[i]);
			pthread_mutex_lock(params->malive);
			params->alive = 0;
			res = 1;
		}
		pthread_mutex_unlock(philo_args[i]->mlast_eat);
		pthread_mutex_unlock(params->malive);
		if (philo_args[i++]->nb_eat != params->nb2eat)
			*nb_alive += 1;
	}
	return (res);
}

int	run_sim(pthread_t **philo, t_params *params)
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
	while (! end && nb_alive)
	{
		nb_alive = 0;
		usleep(500);
		check_philo_exceed_time(philo_args, params, &nb_alive);
		end = ! philo_is_alive(params);
	}
	i = 0;
	while (i < params->nb_philo)
		pthread_join((*philo)[i++], NULL);
	return (cleanup(philo, params, philo_args));
}
