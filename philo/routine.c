/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 09:00:23 by radubos           #+#    #+#             */
/*   Updated: 2025/06/23 02:09:12 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* void print_action(t_philo *philo, const char *msg)
{
    t_data *data = philo->data;
    long   ts;

    pthread_mutex_lock(&data->death_mutex);
    if (!data->someone_died)
    {
        pthread_mutex_lock(&data->print_mutex);
        ts = get_current_time() - data->start_time;
        printf("%ld %d %s\n", ts, philo->id, msg);
        pthread_mutex_unlock(&data->print_mutex);
    }
    pthread_mutex_unlock(&data->death_mutex);
} */

void print_action_ts(t_philo *philo, long ts, const char *msg)
{
    t_data *data = philo->data;

    pthread_mutex_lock(&data->death_mutex);
    if (!data->someone_died)
    {
        pthread_mutex_lock(&data->print_mutex);
        printf("%ld %d %s\n", ts, philo->id, msg);
        pthread_mutex_unlock(&data->print_mutex);
    }
    pthread_mutex_unlock(&data->death_mutex);
}

/* void philo_think(t_philo *philo)
{
    print_action(philo, "is thinking");
} */

void philo_think(t_philo *philo)
{
    t_data *data = philo->data;
    long    ts   = get_current_time() - data->start_time;

    print_action_ts(philo, ts, "is thinking");
}

/* void philo_eat(t_philo *philo)
{
    print_action(philo, "is eating");
    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal  = get_current_time();
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->meal_mutex);
    usleep(philo->data->time_to_eat * 1000);
} */

void philo_eat(t_philo *philo)
{
    t_data *data = philo->data;
    long    now;

    // 1) On marque le début du repas tout de suite
    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal = get_current_time();
    philo->meals_eaten++;
    now = philo->last_meal - data->start_time;
    pthread_mutex_unlock(&philo->meal_mutex);

    // 2) On affiche avec ce timestamp unique
    print_action_ts(philo, now, "is eating");
    usleep(data->time_to_eat * 1000);
}

/* void philo_sleep(t_philo *philo)
{
    print_action(philo, "is sleeping");
    usleep(philo->data->time_to_sleep * 1000);
} */

void philo_sleep(t_philo *philo)
{
    t_data *data = philo->data;
    long    ts   = get_current_time() - data->start_time;

    print_action_ts(philo, ts, "is sleeping");
    usleep(data->time_to_sleep * 1000);
}

/* void take_forks(t_philo *philo)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->right_fork);
        print_action(philo, "has taken a fork");
        pthread_mutex_lock(philo->left_fork);
        print_action(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        print_action(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_action(philo, "has taken a fork");
    }
} */

void take_forks(t_philo *philo)
{
    t_data *data = philo->data;
    long    ts = get_current_time() - data->start_time;

    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->right_fork);
        print_action_ts(philo, ts, "has taken a fork");
        pthread_mutex_lock(philo->left_fork);
        print_action_ts(philo, ts, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        print_action_ts(philo, ts, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_action_ts(philo, ts, "has taken a fork");
    }
}

void drop_forks(t_philo *philo)
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

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data  *data  = philo->data;

    if (philo->id % 2 == 0)
        usleep(1000);
    while (1)
    {
        pthread_mutex_lock(&data->death_mutex);
        if (data->someone_died)
        {
            pthread_mutex_unlock(&data->death_mutex);
            break;
        }
        pthread_mutex_unlock(&data->death_mutex);
        philo_think(philo);
        take_forks(philo);
        philo_eat(philo);
        drop_forks(philo);
        philo_sleep(philo);
    
		if (data->max_meals > 0)
    	{
			pthread_mutex_lock(&philo->meal_mutex);
        	if (philo->meals_eaten >= data->max_meals)
        	{
            	pthread_mutex_unlock(&philo->meal_mutex);
            	break;
        	}
        	pthread_mutex_unlock(&philo->meal_mutex);
		}
    }
    return (NULL);
}

