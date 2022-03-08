/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 21:27:02 by ycornamu          #+#    #+#             */
/*   Updated: 2022/03/08 20:25:50 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

int		philo_search_forks(t_arg *arg);
int		philo_is_alive(t_arg *arg);
void	philo_release_forks(t_arg *arg);
void 	philo_died(t_arg *arg);

void	philo_think(t_arg *arg)
{
	int	has_forks;

	if (! philo_is_alive(arg))
		return ;
	has_forks = 0;
	tprint(arg, "is thinking");
	//usleep(arg->params->t2die * 1000 * 0.3);
	while (! has_forks && philo_is_alive(arg))
		has_forks = philo_search_forks(arg);
}

int	philo_is_alive(t_arg *arg)
{
	pthread_mutex_lock(arg->malive);
	if (! arg->alive)
	{
		pthread_mutex_unlock(arg->malive);
		return(0);
	}
	pthread_mutex_unlock(arg->malive);
	return(1);
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

void 	philo_died(t_arg *arg)
{
	tprint(arg, "\e[0;41mdied\e[0;0m");
	philo_release_forks(arg);
//	pthread_mutex_unlock(arg->alive);
}

int 	philo_search_forks(t_arg *arg)
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

void	philo_release_forks(t_arg *arg)
{
	pthread_mutex_lock(arg->mforkl);
	pthread_mutex_lock(arg->mforkr);
	if (*(arg->forkl) == arg->id)
		*(arg->forkl) = 0;
	if (*(arg->forkr) == arg->id)
		*(arg->forkr) = 0;
	pthread_mutex_unlock(arg->mforkl);
	pthread_mutex_unlock(arg->mforkr);
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
