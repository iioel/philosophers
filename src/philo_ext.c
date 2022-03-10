/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_ext.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 20:16:54 by ycornamu          #+#    #+#             */
/*   Updated: 2022/03/10 20:57:34 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_died(t_arg *arg)
{
	tprint(arg, "\e[0;41mdied\e[0;0m");
	philo_release_forks(arg);
	//pthread_mutex_unlock(arg->alive);
}

int	philo_is_alive(t_arg *arg)
{
	pthread_mutex_lock(arg->malive);
	if (! arg->alive)
	{
		pthread_mutex_unlock(arg->malive);
		return (0);
	}
	pthread_mutex_unlock(arg->malive);
	return (1);
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
