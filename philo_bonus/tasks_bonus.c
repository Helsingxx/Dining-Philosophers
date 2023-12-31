/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 13:40:54 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/31 11:40:06 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	attempt_kill_philosopher(t_arg *arguments_from_argv,
								int current_test_iteration)
{
	t_arg	*ptr;

	ptr = arguments_from_argv;
	if ((current_test_iteration
			&& ((ptr->ate != ptr->to_be_eaten
					&& ptr->to_be_eaten != -1)
				|| ptr->to_be_eaten == -1)) || !ptr->timetodie)
	{
		gettimeofday(&ptr->gtime2, NULL);
		printf("%ld %d has died\n",
			(ptr->gtime2.tv_sec * 1000 + ptr->gtime2.tv_usec / 1000)
			- (ptr->gtime.tv_sec * 1000 + ptr->gtime.tv_usec / 1000),
			ptr->philo_id);
		exit(PHIL_DIED);
	}
	sem_post(ptr->sem_holder->sem_printf);
	return (PHIL_LAP);
}

void	take_otherfork_starteating(t_arg *arguments_from_argv)
{
	t_mysems	*semaphores;
	t_arg		*ptr;

	semaphores = arguments_from_argv->sem_holder;
	ptr = arguments_from_argv;
	sem_wait(ptr->sem_holder->sem_printf);
	gettimeofday(&ptr->gtime2, NULL);
	printf("%ld %d has taken a fork\n", (ptr->gtime2.tv_sec
			- ptr->gtime.tv_sec) * 1000
		+ (ptr->gtime2.tv_usec
			- ptr->gtime.tv_usec) / 1000, ptr->philo_id);
	gettimeofday(&ptr->gtime2, NULL);
	printf("%ld %d is eating\n", (ptr->gtime2.tv_sec
			- ptr->gtime.tv_sec) * 1000
		+ (ptr->gtime2.tv_usec - ptr->gtime.tv_usec)
		/ 1000, ptr->philo_id);
	ptr->lasttimeate = (ptr->gtime2.tv_sec - ptr->gtime.tv_sec) * 1000
		+ (ptr->gtime2.tv_usec - ptr->gtime.tv_usec) / 1000;
	sem_post(ptr->sem_holder->sem_printf);
	chad_wastetime(ptr->timetoeat);
}

int	eating(t_arg *arguments_from_argv)
{
	t_mysems	*semaphores;
	t_arg		*ptr;

	semaphores = arguments_from_argv->sem_holder;
	ptr = arguments_from_argv;
	sem_wait(semaphores->sem_forks_queue);
	sem_wait(ptr->sem_holder->sem_printf);
	gettimeofday(&ptr->gtime2, NULL);
	printf("%ld %d has taken a fork\n", (ptr->gtime2.tv_sec
			- ptr->gtime.tv_sec) * 1000
		+ (ptr->gtime2.tv_usec
			- ptr->gtime.tv_usec) / 1000, ptr->philo_id);
	sem_post(ptr->sem_holder->sem_printf);
	sem_wait(semaphores->sem_forks_queue);
	take_otherfork_starteating(arguments_from_argv);
	sem_post(semaphores->sem_forks_queue);
	sem_post(semaphores->sem_forks_queue);
	sem_wait(ptr->sem_holder->sem_printf);
	ptr->ate++;
	if (ptr->ate == ptr->to_be_eaten)
		return (sem_post(ptr->sem_holder->sem_printf), PHIL_DONE);
	sem_post(ptr->sem_holder->sem_printf);
	return (PHIL_LAP);
}

void	sleeping(t_arg *arguments_from_argv)
{
	t_mysems	*semaphores;
	t_arg		*ptr;

	semaphores = arguments_from_argv->sem_holder;
	ptr = arguments_from_argv;
	sem_wait(ptr->sem_holder->sem_printf);
	gettimeofday(&ptr->gtime2, NULL);
	printf("%ld %d is sleeping\n", (ptr->gtime2.tv_sec
			- ptr->gtime.tv_sec) * 1000
		+ (ptr->gtime2.tv_usec - ptr->gtime.tv_usec) / 1000, ptr->philo_id);
	sem_post(ptr->sem_holder->sem_printf);
	chad_wastetime(ptr->timetosleep);
}

void	thinking(t_arg *arguments_from_argv)
{
	t_mysems	*semaphores;
	t_arg		*ptr;

	semaphores = arguments_from_argv->sem_holder;
	ptr = arguments_from_argv;
	sem_wait(ptr->sem_holder->sem_printf);
	gettimeofday(&ptr->gtime2, NULL);
	printf("%ld %d is thinking\n", (ptr->gtime2.tv_sec
			- ptr->gtime.tv_sec) * 1000
		+ (ptr->gtime2.tv_usec - ptr->gtime.tv_usec) / 1000, ptr->philo_id);
	sem_post(ptr->sem_holder->sem_printf);
}
