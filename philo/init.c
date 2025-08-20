/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 09:01:13 by radubos           #+#    #+#             */
/*   Updated: 2025/06/23 01:37:30 by radubos          ###   ########.fr       */
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
			&data->philos[i]) !=0)
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

static int validate_params(char **argv)
{
    long nb;
    long tdie;
    long teat;
    long tsleep;

    nb = ft_atoi(argv[1]);
    tdie = ft_atoi(argv[2]);
    teat = ft_atoi(argv[3]);
    tsleep = ft_atoi(argv[4]);
    if (nb <= 0 || nb > 200 || tdie <= 0 || tdie > 10000 ||
        teat <= 0 || teat > 10000 || tsleep <= 0 || tsleep > 10000)
    {
        printf("invalid");
        return (1);
    }
    return (0);
}

static void set_data_values(t_data *data, int argc, char **argv)
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

static int init_mutexes(t_data *data)
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

static int	init_data(t_data *data, int argc, char **argv)
{
    if (validate_params(argv))
        return (1);
    set_data_values(data, argc, argv);
    return (init_mutexes(data));
}

static int allocate_resources(t_data *data)
{
    data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
    data->philos = malloc(sizeof(t_philo) * data->nb_philos);
    if (!data->forks || !data->philos)
        return (1);
    return (0);
}

static int init_forks(t_data *data)
{
    int i;

    i = 0;
    while (i < data->nb_philos)
    {
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
            return (1);
        i++;
    }
    return (0);
}

static void set_philo_values(t_data *data, int i)
{
    data->philos[i].id = i + 1;
    data->philos[i].last_meal = data->start_time;
    data->philos[i].meals_eaten = 0;
    data->philos[i].left_fork = &data->forks[i];
    data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philos];
    data->philos[i].data = data;
}

static int init_philo_mutexes(t_data *data)
{
    int i;

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

static int	init_philos(t_data *data)
{
    if (allocate_resources(data))
        return (1);
    if (init_forks(data))
        return (1);
    return (init_philo_mutexes(data));
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
