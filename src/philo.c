/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 21:27:02 by ycornamu          #+#    #+#             */
/*   Updated: 2022/03/10 20:24:33 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

void	philo_think(t_arg *arg)
{
	int	has_forks;

	if (! philo_is_alive(arg))
		return ;
	has_forks = 0;
	tprint(arg, "is thinking");
	while (! has_forks && philo_is_alive(arg))
		has_forks = philo_search_forks(arg);
}

int	philo_search_forks(t_arg *arg)
{
	int	res;

	res = 0;
	pthread_mutex_lock(arg->mforkl);
	pthread_mutex_lock(arg->mforkr);
	if (! *(arg->forkl))
	{
		tprint(arg, "has taken a fork");
		*(arg->forkl) = arg->id;
	}
	if (! *(arg->forkr))
	{
		tprint(arg, "has taken a fork");
		*(arg->forkr) = arg->id;
	}
	if (*(arg->forkl) == arg->id && *(arg->forkr) == arg->id)
		res = 1;
	pthread_mutex_unlock(arg->mforkl);
	pthread_mutex_unlock(arg->mforkr);
	return (res);
}

void	philo_eating(t_arg *arg)
{
	if (! philo_is_alive(arg))
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
	if (! philo_is_alive(arg))
		return ;
	tprint(arg, "is sleeping");
	usleep(arg->params->t2sleep * 1000);
}

void	*run_philo(void *arg_v)
{
	t_arg		*arg;

	arg = (t_arg *)arg_v;
	pthread_mutex_lock(arg->mlast_eat);
	pthread_mutex_lock(arg->malive);
	arg->last_eat = get_time_mili();
	arg->alive = 1;
	pthread_mutex_unlock(arg->mlast_eat);
	pthread_mutex_unlock(arg->malive);
	while (arg->nb_eat != arg->params->nb2eat)
	{
		philo_think(arg);
		philo_eating(arg);
		philo_sleeping(arg);
		if (! philo_is_alive(arg))
		{
			return (NULL);
		}
		arg->nb_eat++;
	}
	pthread_mutex_lock(arg->malive);
	arg->alive = 0;
	pthread_mutex_unlock(arg->malive);
}
