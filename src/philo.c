/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 21:27:02 by ycornamu          #+#    #+#             */
/*   Updated: 2022/03/25 16:57:13 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

void	philo_think(t_arg *arg)
{
	int	has_forks;

	if (! philo_is_alive(arg->params))
		return ;
	has_forks = 0;
	tprint(arg, "is thinking");
	while (! has_forks && philo_is_alive(arg->params))
		has_forks = philo_search_forks(arg);
}

int	philo_search_forks(t_arg *arg)
{
	int	res;

	res = 0;
	if (! *(arg->forkl))
	{
		pthread_mutex_lock(arg->mforkl);
		tprint(arg, "has taken a fork");
		*(arg->forkl) = arg->id;
		pthread_mutex_unlock(arg->mforkl);
	}
	if (! *(arg->forkr))
	{
		pthread_mutex_lock(arg->mforkr);
		tprint(arg, "has taken a fork");
		*(arg->forkr) = arg->id;
		pthread_mutex_unlock(arg->mforkr);
	}
	if (*(arg->forkl) == arg->id && *(arg->forkr) == arg->id
		&& arg->params->nb_philo != 1)
		res = 1;
	return (res);
}

void	philo_eating(t_arg *arg)
{
	if (! philo_is_alive(arg->params))
		return ;
	tprint(arg, "is eating");
	pthread_mutex_lock(arg->mlast_eat);
	arg->last_eat = get_time_mili();
	pthread_mutex_unlock(arg->mlast_eat);
	usleep(arg->params->t2eat * 1000);
	philo_release_forks(arg);
}

void	philo_sleeping(t_arg *arg)
{
	if (! philo_is_alive(arg->params))
		return ;
	tprint(arg, "is sleeping");
	usleep(arg->params->t2sleep * 1000);
}

void	*run_philo(void *arg_v)
{
	t_arg		*arg;

	arg = (t_arg *)arg_v;
	if (! (arg->id % 2))
		usleep(arg->params->t2eat * 1000);
	arg->alive = 1;
	pthread_mutex_lock(arg->mlast_eat);
	arg->last_eat = get_time_mili();
	pthread_mutex_unlock(arg->mlast_eat);
	while (arg->nb_eat != arg->params->nb2eat)
	{
		philo_think(arg);
		philo_eating(arg);
		philo_sleeping(arg);
		if (! philo_is_alive(arg->params))
		{
			return (NULL);
		}
		arg->nb_eat++;
	}
	philo_release_forks(arg);
	return (NULL);
}
