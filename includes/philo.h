/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 19:56:40 by ycornamu          #+#    #+#             */
/*   Updated: 2022/02/11 19:00:38 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

# define THINK 0
# define EAT 1
# define SLEEP 2

typedef struct	s_params
{
	int				nb_philo;
	int 			t2die;
	int 			t2eat;
	int 			t2sleep;
	int 			nb2eat;
	char			*fork;
	pthread_mutex_t *mfork;
	pthread_mutex_t	*mprint;
}				t_params;

typedef struct	s_arg
{
	int				id;
	int				alive;
	pthread_mutex_t	*malive;
	long long int 	last_eat;
	pthread_mutex_t	*mlast_eat;
	int				role;
	char			*forkl;
	char			*forkr;
	pthread_mutex_t	*mforkl;
	pthread_mutex_t	*mforkr;
	t_params		*params;
}				t_arg;

// sim.c
int				run_sim(pthread_t *philo, t_params *params);

// philo.c
void			*run_philo(void *arg_v);
void			*philo_died(t_arg *arg);

// utils.c
int				ft_isdigit(char *str);
void			*ft_calloc(size_t count, size_t size);
int				ft_atoi(char *str);

// time.c
long long int	get_time_mili(void);
void			tprint(int id, char *str);

#endif
