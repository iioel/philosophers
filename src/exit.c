/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 11:07:31 by ycornamu          #+#    #+#             */
/*   Updated: 2022/03/19 12:10:24 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	exit_malloc(void)
{
	printf("error malloc failed\n");
	return (1);
}

int	exit_mutex(void)
{
	printf("error creating mutex failed\n");
	return (1);
}

int	exit_thread(void)
{
	printf("error creating thread failed\n");
	return (1);
}
