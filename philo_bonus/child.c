/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 18:03:45 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/26 22:14:55 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	*philosopher_normal_routine(void *arguments_from_argv)
{
	t_arg	*ptr;

	ptr = (t_arg *)arguments_from_argv;
	if (ptr->ate == ptr->to_be_eaten)
		return (NULL);
	if (ptr->philo_id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (eating(ptr) == PHIL_DONE)
			return (NULL);
		sleeping(ptr);
		thinking(ptr);
	}
}

void	check_phil_finished(t_arg	*ptr)
{
	sem_wait(ptr->sem_holder->sem_printf);
	if (ptr->ate == ptr->to_be_eaten)
	{
		sem_post(ptr->sem_holder->sem_printf);
		exit(PHIL_FINISHED);
	}
	sem_post(ptr->sem_holder->sem_printf);
}

void	*monitor_death_time(void *arguments_from_argv)
{
	struct timeval	timenow;
	int				times;
	int				inc;
	t_arg			*ptr;

	ptr = (t_arg *)arguments_from_argv;
	times = 0;
	inc = 0;
	while (1)
	{
		gettimeofday(&timenow, NULL);
		sem_wait(ptr->sem_holder->sem_printf);
		if ((ptr->timetodie && attempt_kill_philosopher(ptr,
					((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
						- (ptr->gtime.tv_sec * 1000 + ptr->gtime.tv_usec / 1000)
						- ptr->lasttimeate) / ptr->timetodie) == PHIL_LAP)
			|| (!ptr->timetodie && attempt_kill_philosopher(ptr, 1)
				== PHIL_LAP))
			;
		check_phil_finished(ptr);
	}
	return (NULL);
}

int	launch_philosopher_and_monitor(t_arg *arguments_from_argv)
{
	pthread_t	thr_monitor_death_time;
	pthread_t	thr_philosopher;
	int			ret_val;

	ret_val = pthread_create(&thr_monitor_death_time,
			NULL, monitor_death_time, arguments_from_argv);
	if (ret_val)
	{
		printf("An error has occured while trying"
			" to create the monitoring thread!\n");
		return (PHIL_THREAD_FAIL_CREATE);
	}
	ret_val = pthread_create(&thr_philosopher, NULL,
			philosopher_normal_routine, arguments_from_argv);
	if (ret_val)
	{
		printf("An error has occured while"
			" trying to spawn philosopher id=[%d]\n",
			arguments_from_argv->philo_id);
		return (PHIL_THREAD_FAIL_CREATE);
	}
	pthread_join(thr_monitor_death_time, NULL);
	pthread_join(thr_philosopher, NULL);
	return (PHIL_SUCCESS);
}
