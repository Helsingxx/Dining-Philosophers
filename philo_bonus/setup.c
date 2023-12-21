/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 18:00:25 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/21 18:25:11 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int check_program_arguments(int argc, char *argv[])
{
	int scount;
	int eatcnt;

	eatcnt = 0;
	scount = 1;
	if (argc < 5 || argc > 6)
		return (printf("Wrong number of args\n"), 1);
	while (scount < argc)
		if (!is_int0(argv[scount]) || ft_atoi(argv[scount++]) < 0) // test this later
			return (printf("Incorrect arguments\n"), 1);
	return (PHIL_SUCCESS);
}

int init_arg(t_arg *arg, char **argv)
{
	arg = (t_arg *)ft_calloc(sizeof(t_arg), 1);
	arg->argv = argv;
	arg->nbr_philos = ft_atoi(argv[1]);
	arg->timetodie = ft_atoi(argv[2]);
	arg->timetoeat = ft_atoi(argv[3]);
	arg->timetosleep = ft_atoi(argv[4]);
	if (argv[5])
		arg->to_be_eaten = ft_atoi(argv[5]);
	else
		arg->to_be_eaten = -1;
	arg->sem_holder = ft_calloc(sizeof(t_mysems), 1);
	if (!arg->sem_holder)
		return (PHIL_FAIL);
	return (PHIL_SUCCESS);
}

int init_sems(t_arg *argv_parsed)
{
	t_mysems	*ptr;

	ptr = argv_parsed->sem_holder;
	ptr->sem_release_for_exec = sem_open(PHIL_NAME_MONITOR, O_CREAT, 0777, 1);
	if (ptr->sem_release_for_exec == SEM_FAILED)
		return (PHIL_FAIL);
	ptr->sem_printf = sem_open(PHIL_NAME_PRINTF, O_CREAT, 0777, 1);
	if (ptr->sem_printf == SEM_FAILED)
		return (sem_unlink(PHIL_NAME_MONITOR), PHIL_FAIL);
	ptr->sem_forks_queue = sem_open(PHIL_NAME_FORKS, O_CREAT, 0777, argv_parsed->nbr_philos);
	if (ptr->sem_forks_queue == SEM_FAILED)
		return (sem_unlink(PHIL_NAME_MONITOR), sem_unlink(PHIL_NAME_PRINTF), PHIL_FAIL);
	return (PHIL_SUCCESS);
}

void unlink_all_semaphores()
{
	sem_unlink(PHIL_NAME_MONITOR);
	sem_unlink(PHIL_NAME_PRINTF);
	sem_unlink(PHIL_NAME_FORKS);
}

void	*ft_calloc(size_t count, size_t size)
{
	size_t		a;
	char		*ptr;
	int			retry;
	
	a = 0;
	ptr = malloc (count * size);
	retry = 0;
	while (ptr == 0 && count * size && retry < 100)
	{
		printf("Allocation failed of size [%d BYTES] failed, "
				"aborting process in a few retries.\n", count * size);
		ptr = malloc (count * size);
		retry++;	
	}
	if (ptr == 0)
		return (0);
	while (a < count * size)
	{
		ptr[a++] = 0;
	}
	return (ptr);
}

