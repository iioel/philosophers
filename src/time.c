/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 17:31:38 by ycornamu          #+#    #+#             */
/*   Updated: 2022/03/19 11:10:20 by ycornamu         ###   ########.fr       */
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
	long long int	time;

	pthread_mutex_lock(arg->params->mprint);
	time = get_time_mili();
	if (philo_is_alive(arg->params))
		printf("%10ld \e[0;3%dm%3d\e[0;0m %s\n",
			(long int)(time - arg->params->start_time),
			arg->id % 6 + 1, arg->id, str);
	pthread_mutex_unlock(arg->params->mprint);
}
