/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:08:42 by radubos           #+#    #+#             */
/*   Updated: 2025/08/21 16:06:41 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_data(t_data *data)
{
	int	i;

	if (!data)
		return ;
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
	if (data)
		free(data);
}

void	one_philo_case(t_data *data)
{
	printf("0 1 has taken a fork\n");
	usleep(data->time_to_die * 1000);
	printf("%d 1 died\n", data->time_to_die);
	free_data(data);
}

static int	create_monitor_thread(t_data *data, pthread_t *monitor)
{
	if (pthread_create(monitor, NULL, monitor_routine, data) != 0)
	{
		printf("error invalid pthread_create");
		free_data(data);
		return (1);
	}
	return (0);
}

static void	wait_all_threads(t_data *data, pthread_t monitor)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}

int	main(int argc, char **argv)
{
	t_data		*data;
	pthread_t	monitor;

	data = NULL;
	if (validate_and_init(argc, argv, &data))
		return (1);
	if (data->nb_philos == 1)
		return (one_philo_case(data), 0);
	if (create_philo_threads(data))
		return (free_data(data), 1);
	if (create_monitor_thread(data, &monitor))
		return (1);
	wait_all_threads(data, monitor);
	free_data(data);
	return (0);
}
