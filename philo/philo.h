/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:07:53 by radubos           #+#    #+#             */
/*   Updated: 2025/08/21 17:18:28 by radubos          ###   ########.fr       */
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

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	long long		last_meal;
	int				meals_eaten;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	t_data			*data;
}	t_philo;

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

// check.c
int			validate_and_init(int argc, char **argv, t_data **data);
static int	check_args(char **argv);
static int	check_number(char *str);

// init_data.c
static int	validate_params(char **argv);
static void	set_data_values(t_data *data, int argc, char **argv);
static int	init_mutexes(t_data *data);
int			init_data(t_data *data, int argc, char **argv);

// init_philo.c
static int	allocate_resources(t_data *data);
static int	init_forks(t_data *data);
static void	set_philo_values(t_data *data, int i);
static int	init_philo_mutexes(t_data *data);
int			init_philos(t_data *data);

// init.c
int			create_philo_threads(t_data *data);
t_data		*init(t_data *data, int argc, char **argv);

// main.c
static int	create_monitor_thread(t_data *data, pthread_t *monitor);
static void	wait_all_threads(t_data *data, pthread_t monitor);
void		one_philo_case(t_data *data);
void		free_data(t_data *data);

// monitor.c
static int	check_philo_death(t_data *data, int i);
static int	check_all_philos(t_data *data);
void		*monitor_routine(void *arg);

// routine_actions.c
void		philo_think(t_philo *philo);
void		philo_eat(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		take_forks(t_philo *philo);
void		drop_forks(t_philo *philo);

// routine_time.c
void		initial_delay(t_philo *philo);
static void	adaptive_usleep(long remaining);
static void	precise_sleep(t_philo *philo, long duration);

// routine.c
void		print_action_ts(t_philo *philo, long ts, const char *msg);
void		update_meal_info(t_philo *philo);
int			check_death_during_sleep(t_philo *philo);
static int	check_death_and_meals(t_philo *philo);
void		*routine(void *arg);

// utils.c
int			ft_atoi(const char *nptr);
long		get_current_time(void);
size_t		ft_strlen(const char *s);
int			ft_strcmp(const char *s1, const char *s2);

#endif