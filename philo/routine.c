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

void print_action_ts(t_philo *philo, long ts, const char *msg)
{
    t_data *data;

    data = philo->data;
    pthread_mutex_lock(&data->death_mutex);
    if (!data->someone_died)
    {
        pthread_mutex_lock(&data->print_mutex);
        printf("%ld %d %s\n", ts, philo->id, msg);
        pthread_mutex_unlock(&data->print_mutex);
    }
    pthread_mutex_unlock(&data->death_mutex);
}

void philo_think(t_philo *philo)
{
    t_data *data;
    long    ts;

    data = philo->data;
    ts = get_current_time() - data->start_time;
    print_action_ts(philo, ts, "is thinking");
}

static void update_meal_info(t_philo *philo)
{
    t_data *data;
    long    now;

    data = philo->data;
    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal = get_current_time();
    philo->meals_eaten++;
    now = philo->last_meal - data->start_time;
    pthread_mutex_unlock(&philo->meal_mutex);
    print_action_ts(philo, now, "is eating");
}

static void adaptive_usleep(long remaining)
{
    if (remaining > 1000)
        usleep(1000);
    else if (remaining > 100)
        usleep(remaining / 2);
    else
        usleep(50);
}

static int check_death_during_sleep(t_philo *philo)
{
    t_data *data;

    data = philo->data;
    pthread_mutex_lock(&data->death_mutex);
    if (data->someone_died)
    {
        pthread_mutex_unlock(&data->death_mutex);
        return (1);
    }
    pthread_mutex_unlock(&data->death_mutex);
    return (0);
}

static void precise_sleep(t_philo *philo, long duration)
{
    long    start_time;
    long    elapsed;
    long    remaining;

    start_time = get_current_time();
    while (1)
    {
        if (check_death_during_sleep(philo))
            return;
        elapsed = get_current_time() - start_time;
        if (elapsed >= duration)
            break;
        remaining = duration - elapsed;
        adaptive_usleep(remaining);
    }
}

void philo_eat(t_philo *philo)
{
    update_meal_info(philo);
    precise_sleep(philo, philo->data->time_to_eat);
}

void philo_sleep(t_philo *philo)
{
    t_data *data;
    long    ts;

    data = philo->data;
    ts = get_current_time() - data->start_time;
    print_action_ts(philo, ts, "is sleeping");
    precise_sleep(philo, data->time_to_sleep);
}

void take_forks(t_philo *philo)
{
    t_data *data;

    data = philo->data;
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->right_fork);
        print_action_ts(philo, get_current_time() - data->start_time, "has taken a fork");
        pthread_mutex_lock(philo->left_fork);
        print_action_ts(philo, get_current_time() - data->start_time, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        print_action_ts(philo, get_current_time() - data->start_time, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_action_ts(philo, get_current_time() - data->start_time, "has taken a fork");
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

static void initial_delay(t_philo *philo)
{
    t_data *data;

    data = philo->data;
    if (data->nb_philos == 2 && philo->id % 2 == 0)
        usleep(data->time_to_eat * 500);
    else if (philo->id % 2 == 0)
        usleep(1000);
}

static int check_death_and_meals(t_philo *philo)
{
    t_data *data;

    data = philo->data;
    pthread_mutex_lock(&data->death_mutex);
    if (data->someone_died)
    {
        pthread_mutex_unlock(&data->death_mutex);
        return (1);
    }
    pthread_mutex_unlock(&data->death_mutex);
    if (data->max_meals > 0)
    {
        pthread_mutex_lock(&philo->meal_mutex);
        if (philo->meals_eaten >= data->max_meals)
        {
            pthread_mutex_unlock(&philo->meal_mutex);
            return (1);
        }
        pthread_mutex_unlock(&philo->meal_mutex);
    }
    return (0);
}

void *routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    initial_delay(philo);
    while (1)
    {
        if (check_death_and_meals(philo))
            break;
        philo_think(philo);
        take_forks(philo);
        philo_eat(philo);
        drop_forks(philo);
        philo_sleep(philo);
    }
    return (NULL);
}

