/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 19:56:40 by ycornamu          #+#    #+#             */
/*   Updated: 2022/03/10 20:23:29 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

# define THINK 0
# define EAT 1
# define SLEEP 2

typedef struct s_params
{
	int				nb_philo;
	int				t2die;
	int				t2eat;
	int				t2sleep;
	int				nb2eat;
	long long int	start_time;
	char			*fork;
	pthread_mutex_t	*mfork;
	pthread_mutex_t	*mprint;
}				t_params;

typedef struct s_arg
{
	int				id;
	int				alive;
	pthread_mutex_t	*malive;
	int				nb_eat;
	long long int	last_eat;
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
void			philo_think(t_arg *arg);
int				philo_search_forks(t_arg *arg);
void			philo_eating(t_arg *arg);
void			philo_sleeping(t_arg *arg);
void			*run_philo(void *arg_v);

// philo_ext.c
int				philo_is_alive(t_arg *arg);
void			philo_died(t_arg *arg);
void			philo_release_forks(t_arg *arg);

// utils.c
int				ft_isdigit(char *str);
void			*ft_calloc(size_t count, size_t size);
int				ft_atoi(char *str);

// time.c
long long int	get_time_mili(void);
void			tprint(t_arg *arg, char *str);

#endif
