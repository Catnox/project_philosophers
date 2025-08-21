/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:35:14 by radubos           #+#    #+#             */
/*   Updated: 2025/08/21 15:47:24 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_think(t_philo *philo)
{
	t_data	*data;
	long	ts;

	data = philo->data;
	ts = get_current_time() - data->start_time;
	print_action_ts(philo, ts, "is thinking");
}

void	philo_eat(t_philo *philo)
{
	update_meal_info(philo);
	precise_sleep(philo, philo->data->time_to_eat);
}

void	philo_sleep(t_philo *philo)
{
	t_data	*data;
	long	ts;

	data = philo->data;
	ts = get_current_time() - data->start_time;
	print_action_ts(philo, ts, "is sleeping");
	precise_sleep(philo, data->time_to_sleep);
}

void	take_forks(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_action_ts(philo, get_current_time() - data->start_time,
			"has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_action_ts(philo, get_current_time() - data->start_time,
			"has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_action_ts(philo, get_current_time() - data->start_time,
			"has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action_ts(philo, get_current_time() - data->start_time,
			"has taken a fork");
	}
}

void	drop_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
}
