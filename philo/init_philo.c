/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 17:03:12 by radubos           #+#    #+#             */
/*   Updated: 2025/08/21 17:14:25 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	allocate_resources(t_data *data)
{
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->forks || !data->philos)
		return (1);
	return (0);
}

static int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

static void	set_philo_values(t_data *data, int i)
{
	data->philos[i].id = i + 1;
	data->philos[i].last_meal = data->start_time;
	data->philos[i].meals_eaten = 0;
	data->philos[i].left_fork = &data->forks[i];
	data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philos];
	data->philos[i].data = data;
}

static int	init_philo_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		set_philo_values(data, i);
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	init_philos(t_data *data)
{
	if (allocate_resources(data))
		return (1);
	if (init_forks(data))
		return (1);
	return (init_philo_mutexes(data));
}
