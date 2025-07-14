/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namichel <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 03:20:49 by namichel          #+#    #+#             */
/*   Updated: 2025/06/28 02:26:23 by namichel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

struct	s_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	int				someone_died;
	long long		start_time;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	*forks;

	t_philo			*philos;
}					t_data;

/* Function prototypes */
int					init_data(t_data *data, int argc, char **argv, int i);
int					init_philos(t_data *data);
long long			get_time(void);
void				precise_sleep(long long time_ms);
void				*launch_routine(void *arg);
void				print_action(t_philo *philo, char *action);
void				cleanup(t_data *data, int created_threads,
						int initialized_mutexes);
void				dying(t_data *data);
int					protect_death(t_data *data);

#endif
