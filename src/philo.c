/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 21:27:02 by ycornamu          #+#    #+#             */
/*   Updated: 2022/02/26 16:23:56 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

int	philo_search_forks(t_arg *arg);
int	philo_is_alive(t_arg *arg);

void	philo_think(t_arg *arg)
{
	int	has_forks;

	has_forks = 0;
	tprint(arg->id, "is thinking", arg->params->mprint);
	usleep(arg->params->t2die * 0.3);
	while (! has_forks)
	{
		has_forks = philo_search_forks(arg);
		if (! philo_is_alive(arg))
			return ;
	}
}

int	philo_is_alive(t_arg *arg)
{
	pthread_mutex_lock(arg->malive);
	if (! arg->alive)
		return(0);
	pthread_mutex_unlock(arg->malive);
	return(1);
}

void	philo_eating(t_arg *arg)
{
	tprint(arg->id, "is eating", arg->params->mprint);
	usleep(arg->params->t2eat * 1000);
	*(arg->forkl) = 0;
	*(arg->forkr) = 0;
	pthread_mutex_unlock(arg->mforkl);
	pthread_mutex_unlock(arg->mforkr);
}

void	philo_sleeping(t_arg *arg)
{
	tprint(arg->id, "is sleeping", arg->params->mprint);
	usleep(arg->params->t2sleep * 1000);
}

void 	*philo_died(t_arg *arg)
{
	tprint(arg->id, "\033[0;31mdied\033[0;37m", arg->params->mprint);
//	pthread_mutex_unlock(arg->alive);
	return (NULL);
}

int 	philo_search_forks(t_arg *arg)
{
	if (! *(arg->forkl))
	{
		pthread_mutex_lock(arg->mforkl);
		tprint(arg->id, "as taken a fork L", arg->params->mprint);
		*(arg->forkl) = arg->id;
	}
	if (! *(arg->forkr))
	{
		pthread_mutex_lock(arg->mforkr);
		tprint(arg->id, "as taken a fork R", arg->params->mprint);
		*(arg->forkr) = arg->id;
	}
	if (*(arg->forkl) == arg->id && *(arg->forkr) == arg->id)
	{
		return (1);
	}
	return (0);
}

void	*run_philo(void *arg_v)
{
	t_arg		*arg;
	char		has_forks;

	arg = (t_arg *)arg_v;
	pthread_mutex_lock(arg->mlast_eat);
	arg->last_eat = get_time_mili();
	pthread_mutex_unlock(arg->mlast_eat);
	while (1)
	{
		philo_think(arg);
		if (! philo_is_alive(arg))
			return (0);
		philo_eating(arg);
//		tprint(arg->id, "is eating");
//		usleep(arg->params->t2eat * 1000);
//		*(arg->forkl) = 0;
//		*(arg->forkr) = 0;
//		pthread_mutex_unlock(arg->mforkl);
//		pthread_mutex_unlock(arg->mforkr);
		philo_sleeping(arg);
//		tprint(arg->id, "is sleeping");
//		usleep(arg->params->t2sleep * 1000);
	}
}
