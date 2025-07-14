/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namichel <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:42:25 by namichel          #+#    #+#             */
/*   Updated: 2025/06/26 20:42:25 by namichel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	take_forks(t_philo *philo)
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
}

static void	one_philo(t_philo *philos)
{
	pthread_mutex_lock(philos->left_fork);
	print_action(philos, "has taken a fork");
	pthread_mutex_lock(&philos->data->death_mutex);
	usleep(philos->data->time_to_die * 1000);
	print_action(philos, "died");
	pthread_mutex_unlock(philos->left_fork);
	philos->data->someone_died = 1;
	pthread_mutex_unlock(&philos->data->death_mutex);
}

static void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	routine(t_philo *philo)
{
	long long	now;

	while (!protect_death(philo->data))
	{
		print_action(philo, "is thinking");
		take_forks(philo);
		now = get_time();
		pthread_mutex_lock(&philo->data->death_mutex);
		philo->last_meal_time = now;
		pthread_mutex_unlock(&philo->data->death_mutex);
		print_action(philo, "is eating");
		precise_sleep(philo->data->time_to_eat);
		pthread_mutex_lock(&philo->data->death_mutex);
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->data->death_mutex);
		release_forks(philo);
		if (philo->data->max_meals != -1
			&& philo->meals_eaten >= philo->data->max_meals)
			break ;
		print_action(philo, "is sleeping");
		precise_sleep(philo->data->time_to_sleep);
	}
}

void	*launch_routine(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	if (philo->data->nb_philos == 1)
	{
		one_philo(philo);
		return (NULL);
	}
	usleep(philo->id * 1000);
	routine(philo);
	return (NULL);
}
