/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:35:14 by radubos           #+#    #+#             */
/*   Updated: 2025/08/22 19:40:16 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_think(t_philo *philo)
{
	print_action_ts(philo, "is thinking");
}

void	philo_eat(t_philo *philo)
{
	update_meal_info(philo);
	ft_usleep(philo->data->time_to_eat);
}

void	philo_sleep(t_philo *philo)
{
	print_action_ts(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_action_ts(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_action_ts(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_action_ts(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action_ts(philo, "has taken a fork");
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
