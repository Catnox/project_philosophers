/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:40:01 by radubos           #+#    #+#             */
/*   Updated: 2025/08/22 19:47:44 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_philo_death(t_data *data, int i)
{
	long	time_since_last;

	pthread_mutex_lock(&data->philos[i].meal_mutex);
	time_since_last = get_current_time() - data->philos[i].last_meal;
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	if (time_since_last > data->time_to_die)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (!data->someone_died)
		{
			data->someone_died = 1;
			pthread_mutex_lock(&data->print_mutex);
			printf("%ld %d died\n", get_current_time() - data->start_time,
				data->philos[i].id);
			pthread_mutex_unlock(&data->print_mutex);
		}
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}

static int	check_all_meals_eaten(t_data *data)
{
	int	i;
	int	all_finished;

	if (data->max_meals <= 0)
		return (0);
	all_finished = 1;
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->philos[i].meals_eaten < data->max_meals)
			all_finished = 0;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		if (!all_finished)
			break ;
		i++;
	}
	if (all_finished)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}

static int	check_all_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (check_philo_death(data, i))
			return (1);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->someone_died)
		{
			pthread_mutex_unlock(&data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->death_mutex);
		if (check_all_meals_eaten(data))
			return (NULL);
		if (check_all_philos(data))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
