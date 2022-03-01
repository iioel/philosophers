/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 17:31:38 by ycornamu          #+#    #+#             */
/*   Updated: 2022/03/01 22:07:31 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <sys/time.h>

long long int	get_time_mili(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * (long long int)1000 + tv.tv_usec / 1000);
}

void	tprint(t_arg *arg, char *str)
{
	pthread_mutex_lock(arg->params->mprint);
	printf("%10d \e[0;3%dm%d\e[0;0m %s\n", get_time_mili() - arg->params->start_time, arg->id % 7, arg->id, str);
	pthread_mutex_unlock(arg->params->mprint);
}

