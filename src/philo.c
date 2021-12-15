/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 21:27:02 by ycornamu          #+#    #+#             */
/*   Updated: 2021/12/15 01:50:03 by yoel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

suseconds_t	get_ts_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_usec);
}

void	tprint(int id, char *str)
{
	printf("%ld %d %s\n", get_ts_us(), id, str);
}

void	philo_eating(t_arg *arg)
{
	tprint(arg->id, "is eating");
	usleep(arg->params->t2eat);
	*(arg->forkl) = 0;
	*(arg->forkr) = 0;
	pthread_mutex_unlock(arg->mforkl);
	pthread_mutex_unlock(arg->mforkr);
	arg->role = SLEEP;
}

void	philo_sleeping(t_arg *arg)
{
	tprint(arg->id, "is sleeping");
	usleep(arg->params->t2sleep);
	arg->role = THINK;
}

void 	*philo_died(t_arg *arg)
{
	tprint(arg->id, "\033[0;31mdied\033[0;37m");
//	pthread_mutex_unlock(arg->alive);
	return (NULL);
}

int 	philo_search_forks(t_arg *arg)
{
	static char	fork[2];
	if (! *(arg->forkl) && ! pthread_mutex_lock(arg->mforkl))
	{
		tprint(arg->id, "as taken a fork");
		*(arg->forkl) = arg->id;
	}
	if (! *(arg->forkr) && ! pthread_mutex_lock(arg->mforkr))
	{
		tprint(arg->id, "as taken a fork");
		*(arg->forkr) = arg->id;
	}
	if (*(arg->forkl) == arg->id && *(arg->forkr) == arg->id)
	{
		arg->role = EAT;
		return (2);
	}
	return (0);
}

void	*run_philo(void *arg_v)
{
	t_arg		*arg;
	suseconds_t	time_start;

	arg = (t_arg *)arg_v;
//	pthread_mutex_lock(arg->alive);
	while (1)
	{
//		if (arg->role == THINK)
//		{
			time_start = get_ts_us();
			tprint(arg->id, "is thinking");
			usleep(arg->params->t2die * 0.7);
			while (1)
			{
				if (philo_search_forks(arg) == 2)
					break ;
				if (time_start + arg->params->t2die >= get_ts_us())
					return (philo_died(arg));
			}
//		}
//		if (arg->role == EAT)
			philo_eating(arg);
//		if (arg->role == SLEEP)
			philo_sleeping(arg);
	}
}
