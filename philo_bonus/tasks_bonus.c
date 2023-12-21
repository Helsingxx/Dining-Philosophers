/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 13:40:54 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/21 17:56:17 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	attempt_kill_philosopher(t_arg *arguments_from_argv, int current_test_iteration)
{
	t_arg			*ptr;

	ptr = (t_arg *)arguments_from_argv;
	if ((ptr->ate < current_test_iteration
		&& ((ptr->ate != ptr->to_be_eaten
		&& ptr->to_be_eaten != -1)
		|| ptr->to_be_eaten == -1))
		|| !ptr->timetodie)
	{
		sem_wait(&ptr->sem_holder->sem_printf);
		printf("%d %d has died %d - %d\n", 
				current_test_iteration * ptr->timetodie, ptr->philo_id,
				ptr->ate, current_test_iteration);
		exit(PHIL_DIED);
	}
	return (PHIL_LAP);
}

void take_otherfork_starteating(t_arg *arguments_from_argv)
{
	t_mysems	*semaphores;
	t_arg		*ptr;

	semaphores = arguments_from_argv->sem_holder;
	ptr = arguments_from_argv;
	sem_wait(&semaphores->sem_printf);
	{
		gettimeofday(&ptr->gtime2, NULL);
		printf("%ld %d has taken a fork\n", (ptr->gtime2.tv_sec - ptr->gtime.tv_sec) * 1000 +
									(ptr->gtime2.tv_usec - ptr->gtime.tv_usec) / 1000, ptr->philo_id);
	}
	{
		gettimeofday(&ptr->gtime2, NULL);
		printf("%ld %d is eating\n", (ptr->gtime2.tv_sec - ptr->gtime.tv_sec) * 1000 +
									(ptr->gtime2.tv_usec - ptr->gtime.tv_usec) / 1000, ptr->philo_id);
	}
	sem_post(&semaphores->sem_printf);
	chad_wastetime(ptr->timetoeat);
}

int eating(t_arg *arguments_from_argv)
{
	t_mysems	*semaphores;
	t_arg		*ptr;

	semaphores = arguments_from_argv->sem_holder;
	ptr = arguments_from_argv;
	{
		sem_wait(&semaphores->sem_forks_queue);
		sem_wait(&semaphores->sem_printf);
		gettimeofday(&ptr->gtime2, NULL);
		printf("%ld %d has taken a fork\n", (ptr->gtime2.tv_sec - ptr->gtime.tv_sec) * 1000 +
									(ptr->gtime2.tv_usec - ptr->gtime.tv_usec) / 1000, ptr->philo_id);
		sem_post(&semaphores->sem_printf);
		sem_wait(&semaphores->sem_forks_queue);
		take_otherfork_starteating(arguments_from_argv);
	}
	{
		sem_post(&semaphores->sem_forks_queue);
		sem_post(&semaphores->sem_forks_queue);
		ptr->ate++;
	}
	if (ptr->ate == ptr->to_be_eaten)
		return (PHIL_DONE);
	return (PHIL_LAP);
}

void sleeping(t_arg *arguments_from_argv)
{
	t_mysems	*semaphores;
	t_arg		*ptr;

	semaphores = arguments_from_argv->sem_holder;
	ptr = arguments_from_argv;
	{
		sem_wait(&semaphores->sem_printf);
		gettimeofday(&ptr->gtime2, NULL);
		printf("%ld %d is sleeping\n", (ptr->gtime2.tv_sec - ptr->gtime.tv_sec) * 1000 +
									(ptr->gtime2.tv_usec - ptr->gtime.tv_usec) / 1000, ptr->philo_id);
		sem_post(&semaphores->sem_printf);
	}
	chad_wastetime(ptr->timetosleep);
}

void thinking(t_arg *arguments_from_argv)
{
	t_mysems	*semaphores;
	t_arg		*ptr;

	semaphores = arguments_from_argv->sem_holder;
	ptr = arguments_from_argv;
	sem_wait(&semaphores->sem_printf);
	gettimeofday(&ptr->gtime2, NULL);
	printf("%ld %d is thinking\n", (ptr->gtime2.tv_sec - ptr->gtime.tv_sec) * 1000 +
								(ptr->gtime2.tv_usec - ptr->gtime.tv_usec) / 1000, ptr->philo_id);
	sem_post(&semaphores->sem_printf);
}

void chad_wastetime(int span_in_milliseconds)
{
	struct timeval time_now;
	struct timeval initial_time;
	int x;
	int c;

	c = 1;
	x = 0;
	gettimeofday(&initial_time, NULL);
	gettimeofday(&time_now, NULL);
	while (((time_now.tv_sec * 1000 + time_now.tv_usec / 1000)
			- (initial_time.tv_sec * 1000 + initial_time.tv_usec / 1000)) < span_in_milliseconds)
	{
		usleep(10);
		x++;
		gettimeofday(&time_now, NULL);
	}
}