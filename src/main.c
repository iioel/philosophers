/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 19:54:47 by ycornamu          #+#    #+#             */
/*   Updated: 2021/12/15 01:13:51 by yoel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

static int	init_params(t_params *params, int ac, char *av[])
{
	if ((ac == 4 || ac == 5) && ft_isdigit(av[0]) && ft_isdigit(av[1])
		&& ft_isdigit(av[2]) && ft_isdigit(av[3]))
	{
		params->nb_philo = ft_atoi(av[0]);
		params->t2die = ft_atoi(av[1]);
		params->t2eat = ft_atoi(av[2]);
		params->t2sleep = ft_atoi(av[3]);
		if (ac == 5)
		{
			if (ft_isdigit(av[4]))
				params->nb2eat = ft_atoi(av[4]);
			else
				return (1);
		}
	}
	else
		return (1);
	return (0);
}

static int init_forks(t_params *params, int nb)
{
	int i;

	i = 0;
	params->mfork = ft_calloc(nb, sizeof(pthread_mutex_t));
	params->fork = ft_calloc(nb, sizeof(char));
	if (! params->mfork || ! params->fork)
		return (1);
	while (i < nb)
	{
		if (pthread_mutex_init(&(params->mfork[i++]), NULL))
		{
			printf("error mutex failed\n");
			return (1);
		}
	}
	return (0);
}

int	main(int ac, char *av[])
{
	int				i;
	t_params		params;
	pthread_t		*philo;

	if (init_params(&params, ac - 1, av + 1))
//		return (print_usage());
		return (1);
	if (init_forks(&params, params.nb_philo))
		return (1);
	run_sim(philo, &params);
}
