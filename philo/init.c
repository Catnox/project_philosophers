/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 09:01:13 by radubos           #+#    #+#             */
/*   Updated: 2025/08/21 17:14:09 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philo_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, routine,
				&data->philos[i]) != 0)
		{
			printf("error invalid pthread_create");
			data->someone_died = 1;
			free_data(data);
			return (1);
		}
		i++;
	}
	return (0);
}

t_data	*init(t_data *data, int argc, char **argv)
{
	data = malloc(sizeof(t_data));
	if (!data)
		return (write(STDERR_FILENO, "Error invalid malloc\n", 13), NULL);
	if (init_data(data, argc, argv) != 0)
	{
		free(data);
		return (NULL);
	}
	if (init_philos(data) != 0)
	{
		free(data->forks);
		free(data->philos);
		pthread_mutex_destroy(&data->print_mutex);
		pthread_mutex_destroy(&data->death_mutex);
		free(data);
		return (NULL);
	}
	return (data);
}
