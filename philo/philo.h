/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:07:53 by radubos           #+#    #+#             */
/*   Updated: 2025/06/23 02:07:09 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <errno.h>
# include <limits.h>

typedef struct s_data t_data;

typedef struct s_philo
{
	int				id;
	long long		last_meal;
	int				meals_eaten;
	pthread_t		thread;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	pthread_mutex_t	meal_mutex;
	t_data			*data;
} t_philo;

struct s_data
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	int				someone_died;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	t_philo			*philos;
};

t_data	*init(t_data *data, int argc, char **argv);
int		ft_atoi(const char *nptr);
void	*routine(void *arg);
long	get_current_time(void);
void	*monitor_routine(void *arg);
void    one_philo_case(t_data *data);
int		create_philo_threads(t_data *data);
void    free_data(t_data *data);
size_t	ft_strlen(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
void    print_action_ts(t_philo *philo, long ts, const char *msg);

#endif