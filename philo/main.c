/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:08:42 by radubos           #+#    #+#             */
/*   Updated: 2025/06/23 01:42:16 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int check_number(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    i = 0;
	while (str[i] == '0')
		i++;
	if (!str[i])
		return (0);
	if (ft_strlen(str + i) < 10)
		return (1);
	else if (ft_strlen(str + i) > 10)
		return (0);
	else
		return (ft_strcmp(str + i, "2147483647") <= 0);
}

static int check_args(char **argv)
{
    int i;

    i = 1;
    while (argv[i])
    {
        if (!check_number(argv[i]))
            return (0);
        i++;
    }
    return (1);
}

void    free_data(t_data *data)
{
    int i;

    if (!data)
        return;
    i = 0;
    while (i < data->nb_philos)
    {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->death_mutex);
    pthread_mutex_destroy(&data->philos->meal_mutex);
    if (data->forks)
        free(data->forks);
    if (data->philos)
        free(data->philos);
    if (data)
        free(data);
}

void    one_philo_case(t_data *data)
{
    printf("0 1 has taken a fork\n");
    usleep(data->time_to_die * 1000);
    printf("%d 1 died\n", data->time_to_die);
    free_data(data);
}

int	main(int argc, char **argv)
{
    t_data *data;
    pthread_t monitor;
    int     i;
    
    i = 0;
    data = NULL;
    if (argc < 5 || argc > 6 || !check_args(argv))
        return (write(STDERR_FILENO, "Error invalid\n", 14), 1);
    if (!(data = init(data, argc, argv)))
        return (1);
    if (data->nb_philos == 1)
        return (one_philo_case(data), 0);
    if (create_philo_threads(data))
    {
        return (free_data(data), 1);
    }
	if (pthread_create(&monitor, NULL, monitor_routine, data) != 0)
        printf("error invalid pthread_create");  
    i = 0;
    while (i < data->nb_philos)
    {
        pthread_join(data->philos[i].thread, NULL);
        i++;
    }
    pthread_join(monitor, NULL);
    free_data(data);
    return (0);
}
