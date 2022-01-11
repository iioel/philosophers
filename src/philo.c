/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 21:27:02 by ycornamu          #+#    #+#             */
/*   Updated: 2022/01/11 15:13:35 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

long long int	get_time_mili(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * (long long int)1000 + tv.tv_usec / 1000);
}

void	tprint(int id, char *str)
{
	printf("%ld %d %s\n", get_time_mili(), id, str);
}

void	philo_eating(t_arg *arg)
{
	tprint(arg->id, "is eating");
	usleep(arg->params->t2eat * 1000);
	*(arg->forkl) = 0;
	*(arg->forkr) = 0;
	pthread_mutex_unlock(arg->mforkl);
	pthread_mutex_unlock(arg->mforkr);
	arg->role = SLEEP;
}

void	philo_sleeping(t_arg *arg)
{
	tprint(arg->id, "is sleeping");
	usleep(arg->params->t2sleep * 1000);
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
//	static char	fork[2];
	if (! *(arg->forkl))
//	if (! pthread_mutex_lock(arg->mforkl))
	{
		pthread_mutex_lock(arg->mforkl);
		tprint(arg->id, "as taken a fork L");
		*(arg->forkl) = arg->id;
	}
	if (! *(arg->forkr))
//	if (! pthread_mutex_lock(arg->mforkr))
	{
		pthread_mutex_lock(arg->mforkr);
		tprint(arg->id, "as taken a fork R");
		*(arg->forkr) = arg->id;
	}
	if (*(arg->forkl) == arg->id && *(arg->forkr) == arg->id)
	{
		return (1);
	}
	//printf("Not enought forks !\n");
	return (0);
}

void	*run_philo(void *arg_v)
{
	t_arg		*arg;
	char		has_forks;
	long long 	last_eat;

	arg = (t_arg *)arg_v;
//	pthread_mutex_lock(arg->alive);
	last_eat = get_time_mili();
	while (1)
	{
		if (arg->role == THINK)
		{
			tprint(arg->id, "is thinking");
			//printf("start: %ld\n", time_start);
			//usleep(arg->params->t2die * 0.3);
			while (arg->role == THINK)
			{
				has_forks = philo_search_forks(arg);
				if (last_eat + arg->params->t2die < get_time_mili())
				{
					printf("end: %ld\n", last_eat + arg->params->t2die);
					return (philo_died(arg));
				}
				if (has_forks)
					arg->role = EAT;
			}
		}
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
