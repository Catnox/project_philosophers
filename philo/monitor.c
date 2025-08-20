/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:40:01 by radubos           #+#    #+#             */
/*   Updated: 2025/06/23 00:34:17 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *monitor_routine(void *arg) {
    t_data *data = (t_data *)arg;
    int i;

    while (1) {
		pthread_mutex_lock(&data->death_mutex);
        if (data->someone_died)
        {
            pthread_mutex_unlock(&data->death_mutex);
            break;
        }
        pthread_mutex_unlock(&data->death_mutex);
        for (i = 0; i < data->nb_philos; i++) {
			pthread_mutex_lock(&data->philos[i].meal_mutex);
            long time_since_last = get_current_time() - data->philos[i].last_meal;
            pthread_mutex_unlock(&data->philos[i].meal_mutex);
            if (time_since_last > data->time_to_die)
            {
                pthread_mutex_lock(&data->death_mutex);
                data->someone_died = 1;
                pthread_mutex_unlock(&data->death_mutex);
                pthread_mutex_lock(&data->print_mutex);
                long ts = get_current_time() - data->start_time;
                printf("%ld %d died\n", ts, data->philos[i].id);
                pthread_mutex_unlock(&data->print_mutex);
                return (NULL);
            }
        }
        pthread_mutex_unlock(&data->print_mutex);
        usleep(1000);
    }
    return (NULL);
}
