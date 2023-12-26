/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:42:33 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/26 22:06:01 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned long			a;
	char					*ptr;

	a = 0;
	ptr = malloc(count * size);
	if (ptr == 0)
		return (0);
	while (a < count * size)
	{
		ptr[a++] = 0;
	}
	return (ptr);
}

int	parse(int argc, char *argv[])
{
	int	scount;
	int	eatcnt;

	eatcnt = 0;
	scount = 1;
	if (argc < 5 || argc > 6)
		return (printf("Wrong number of args\n"), 1);
	while (scount < argc)
		if (!is_int0(argv[scount]) || ft_atoi(argv[scount++]) < 0)
			return (printf("Incorrect arguments\n"), 1);
	return (SUCCESS);
}

int	init_arg(t_arg *arg, char **argv)
{
	arg->argv = argv;
	arg->nbr_philos = ft_atoi(argv[1]);
	arg->timetodie = ft_atoi(argv[2]);
	arg->timetoeat = ft_atoi(argv[3]);
	arg->timetosleep = ft_atoi(argv[4]);
	if (argv[5])
		arg->to_be_eaten = ft_atoi(argv[5]);
	else
		arg->to_be_eaten = -1;
	arg->ate = (int *) ft_calloc(sizeof(int), arg->nbr_philos);
	if (!arg->ate)
		return (FAIL);
	arg->lock = (pthread_mutex_t *) ft_calloc(sizeof(pthread_mutex_t),
			arg->nbr_philos + 1);
	if (!arg->lock)
		return (free(arg->ate), FAIL);
	arg->lasttimeate = ft_calloc(sizeof(int), arg->nbr_philos);
	if (!arg->lasttimeate)
		return (free(arg->ate), free(arg->lock), FAIL);
	return (SUCCESS);
}

int	init_mutexes(t_arg	*arg)
{
	int	x;

	x = 0;
	if (pthread_mutex_init(&arg->lock_readygo, NULL))
		return (FAIL);
	if (pthread_mutex_init(&arg->lock_printf, NULL))
		return (pthread_mutex_destroy(&arg->lock_readygo),
			FAIL);
	if (pthread_mutex_init(&arg->lock_finish, NULL))
		return (pthread_mutex_destroy(&arg->lock_readygo),
			pthread_mutex_destroy(&arg->lock_printf),
			FAIL);
	while (x < arg->nbr_philos + 1)
		if (pthread_mutex_init(&arg->lock[x++], NULL))
			return (__fmutexes(arg->lock, x),
				pthread_mutex_destroy(&arg->lock_readygo),
				pthread_mutex_destroy(&arg->lock_printf),
				pthread_mutex_destroy(&arg->lock_finish),
				FAIL);
	return (SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_arg		*arg;
	pthread_t	*threads;

	arg = ft_calloc(sizeof(t_arg), 1);
	if (parse(argc, argv))
		return (FAIL);
	if (!ft_atoi(argv[1]))
		return (printf("Null simulation"), FAIL);
	if (ft_atoi(argv[1]) > 200)
		return (printf("The system would not support running"
				" that number of threads!\n"), FAIL);
	threads = ft_calloc(sizeof(pthread_t), ft_atoi(argv[1]) + 1 + 3);
	if (!threads)
		return (FAIL);
	if (init_arg(arg, argv) == FAIL)
		return (free(threads), FAIL);
	if (init_mutexes(arg) == FAIL)
		return (__freeall(threads, arg), FAIL);
	wait_for_threads(launch_threads(threads, argv, arg), arg, threads, argv);
	return (__fmutexes(arg->lock, arg->nbr_philos),
		pthread_mutex_destroy(&arg->lock_readygo),
		pthread_mutex_destroy(&arg->lock_printf),
		pthread_mutex_destroy(&arg->lock_finish),
		__freeall(threads, arg),
		SUCCESS);
}
