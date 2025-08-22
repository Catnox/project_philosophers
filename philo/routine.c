/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 09:00:23 by radubos           #+#    #+#             */
/*   Updated: 2025/08/22 20:01:23 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action_ts(t_philo *philo, const char *msg)
{
	t_data	*data;
	long	timestamp;

	data = philo->data;
	pthread_mutex_lock(&data->death_mutex);
	if (!data->someone_died)
	{
		pthread_mutex_lock(&data->print_mutex);
		timestamp = get_current_time() - data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, msg);
		pthread_mutex_unlock(&data->print_mutex);
	}
	pthread_mutex_unlock(&data->death_mutex);
}

void	update_meal_info(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_action_ts(philo, "is eating");
}

int	check_death_during_sleep(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->death_mutex);
	if (data->someone_died)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}

static int	should_continue(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	if (philo->data->max_meals > 0)
	{
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->meals_eaten >= philo->data->max_meals)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			return (0);
		}
		pthread_mutex_unlock(&philo->meal_mutex);
	}
	return (1);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	initial_delay(philo);
	while (should_continue(philo))
	{
		philo_think(philo);
		take_forks(philo);
		philo_eat(philo);
		drop_forks(philo);
		philo_sleep(philo);
	}
	return (NULL);
}
