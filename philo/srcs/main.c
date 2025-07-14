/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namichel <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 03:39:52 by namichel          #+#    #+#             */
/*   Updated: 2025/06/24 03:39:52 by namichel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	cleanup(t_data *data, int created_threads, int init_mutexes)
{
	int	i;

	i = 0;
	while (i < created_threads)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < init_mutexes)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}

int	protect_death(t_data *data)
{
	int	value;

	pthread_mutex_lock(&data->death_mutex);
	value = data->someone_died;
	pthread_mutex_unlock(&data->death_mutex);
	return (value);
}

int	main(int argc, char **argv)
{
	t_data		data;
	int			i;

	if (init_data(&data, argc, argv, 0) != 0)
		return (1);
	data.start_time = get_time();
	if (init_philos(&data) != 0)
		return (1);
	i = 0;
	while (i < data.nb_philos)
	{
		if (pthread_create(&data.philos[i].thread, NULL, launch_routine,
				&data.philos[i]) != 0)
		{
			cleanup(&data, --i, data.nb_philos);
			write(2, "Error: failed thread\n", 21);
			return (1);
		}
		++i;
	}
	dying(&data);
	cleanup(&data, data.nb_philos, data.nb_philos);
	return (0);
}
