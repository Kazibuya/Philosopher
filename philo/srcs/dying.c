/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dying.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namichel <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 02:06:32 by namichel          #+#    #+#             */
/*   Updated: 2025/06/29 02:06:32 by namichel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	death(t_data *data, int i)
{
	long long	now;
	long long	time_since_last_meal;

	pthread_mutex_lock(&data->death_mutex);
	now = get_time();
	time_since_last_meal = now - data->philos[i].last_meal_time;
	if (time_since_last_meal > data->time_to_die)
	{
		print_action(&data->philos[i], "died");
		data->someone_died = 1;
	}
	pthread_mutex_unlock(&data->death_mutex);
}

static void	check_meals(t_data *data, int *all_full, int i)
{
	pthread_mutex_lock(&data->death_mutex);
	if (data->max_meals != -1
		&& data->philos[i].meals_eaten < data->max_meals)
		*all_full = 0;
	pthread_mutex_unlock(&data->death_mutex);
}

void	dying(t_data *data)
{
	int			i;
	int			all_full;

	while (!protect_death(data))
	{
		i = 0;
		while (i < data->nb_philos)
		{
			death(data, i);
			i++;
		}
		all_full = 1;
		i = 0;
		while (i < data->nb_philos)
		{
			check_meals(data, &all_full, i);
			i++;
		}
		if (all_full && data->max_meals != -1)
			data->someone_died = 1;
		usleep(100);
	}
}
