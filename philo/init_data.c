/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 17:02:49 by radubos           #+#    #+#             */
/*   Updated: 2025/08/21 17:19:25 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	validate_params(char **argv)
{
	long	nb;
	long	tdie;
	long	teat;
	long	tsleep;

	nb = ft_atoi(argv[1]);
	tdie = ft_atoi(argv[2]);
	teat = ft_atoi(argv[3]);
	tsleep = ft_atoi(argv[4]);
	if (nb <= 0 || nb > 200 || tdie <= 0 || tdie > 10000 || teat <= 0
		|| teat > 10000 || tsleep <= 0 || tsleep > 10000)
	{
		printf("invalid");
		return (1);
	}
	return (0);
}

static void	set_data_values(t_data *data, int argc, char **argv)
{
	data->nb_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->max_meals = ft_atoi(argv[5]);
	else
		data->max_meals = -1;
	data->someone_died = 0;
	data->start_time = get_current_time();
}

static int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (1);
	}
	return (0);
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (validate_params(argv))
		return (1);
	set_data_values(data, argc, argv);
	return (init_mutexes(data));
}
