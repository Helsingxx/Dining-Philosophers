/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:42:33 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/21 13:27:28 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void  *monitor_death_time(void *arguments_from_argv)
{
	struct timeval timenow;
	int				times;
	int				inc;
	t_arg			*ptr;

	ptr = (t_arg *)arguments_from_argv;
	times = 0;
	inc = 0;
	sem_wait(&ptr->sem_holder->sem_release_for_exec);
	sem_post(&ptr->sem_holder->sem_release_for_exec);
	while (1)
	{
		gettimeofday(&timenow, NULL);
		if ((philo_infos->timetodie && kill_him(philo_infos, ((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
				- (philo_infos->gtime.tv_sec * 1000 + philo_infos->gtime.tv_usec / 1000)) / philo_infos->timetodie) == FAIL) || (!philo_infos->timetodie
				&& kill_him(philo_infos, 1) == FAIL))
			return (NULL);
	}
	return (NULL);
}

void *common_routine(void *arg)
{
	if (conv->ate[philo_id - 1] == conv->to_be_eaten)
		return (NULL);
	if (philo_id % 2 == 0)
		usleep(100); // pay attention
	while (1)
	{
		if (eating(conv, philo_id, next) == DONE)
			return (NULL);
		if (sleeping(conv, philo_id) == EXIT)
			return (NULL);
		if (thinking(conv, philo_id) == EXIT)
			return (NULL);
	}
}

void __freeall(pthread_t *threads, t_arg *arg)
{
	int x;

	x = 0;
	free(arg->ate);
	free(arg->lock);
	free(threads);
}

void	*ft_calloc(size_t count, size_t size)
{
	unsigned long long		a;
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
	return (SUCCESS);
}

void __fargs(t_arg **args, int cnt)
{
	int x;

	x = 0;
	while (x < cnt)
		free(args[x++]);
	free(args);
} // to verify

//int max_arr(int *arr, int len)
//{
//	int x;
//	int max;
//	
//	x = 0;
//	max = arr[0];
//	while (x < len)
//	{
//		if (max < arr[x])
//			max = arr[x];
//		x++;
//	}
//	return (max);
//}

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
	arg->ate = (int *) ft_calloc(sizeof(int), arg->nbr_philos);
	if (!arg->ate)
		return (FAIL);
	arg->sem_holder = ft_calloc(sizeof(t_mysems), 1);
	if (!arg->sem_holder)
		return (free(arg->ate), FAIL);
	return (SUCCESS);
}

void launch_monitor(t_arg *arguments_from_argv)
{
	pthread_t	thr_monitor_death_time;
	int			ret_val;

	ret_val = pthread_create(&thr_monitor_death_time, NULL, monitor_death_time, arguments_from_argv);
	if (ret_val)
	{
		printf("An error has occured while trying to create the monitoring thread!\n");
		return (THREAD_FAIL_CREATE);
	}
}

void failure_create_process(int ecx, int *launched_child_pids)
{
	int loop;

	loop = 0;
	while (loop < ecx)
		kill(SIGTERM, launched_child_pids[ecx++]);
}

void set_global_timer(t_arg *arguments_from_argv)
{
	t_arg *ptr;
	
	ptr = arguments_from_argv;
	gettimeofday(&ptr->gtime, NULL);
	ptr->is_gtime_set = 1;
}

void philosopher_normal_routine(t_arg *arguments_from_argv)
{
	t_arg *ptr;

	ptr = arguments_from_argv;
	if (ptr->ate[philo_id - 1] == ptr->to_be_eaten)
		return (NULL);
	
	sem_wait(&ptr->sem_holder->sem_release_for_exec);
	if (!ptr->is_gtime_set)
		set_global_timer(ptr);
	sem_post(&ptr->sem_holder->sem_release_for_exec);
}

void fork_handler(t_arg* arguments_from_argv, pid_t chld_pid, int ecx, int *launched_child_pids)
{
	if (chld_pid == -1)
		failure_create_process(ecx, launched_child_pids);
	if (!chld_pid)
	{
		launch_monitor(arguments_from_argv);
		philosopher_normal_routine(arguments_from_argv);
	}
}

void wait_for_childs(int *launched_child_pids)
{
	int exit_status;

	while (waitpid(-1, &exit_status, WEXITED) != -1)
		;
}

int launch_processes(t_arg *arguments_from_argv)
{
	int *launched_child_pids;
	pid_t chld_pid;
	int ecx;

	launched_child_pids = (int *) ft_calloc(sizeof(int), arguments_from_argv->nbr_philos);
	ecx = 0;
	chld_pid = -1;
	sem_wait(&arguments_from_argv->sem_holder->sem_release_for_exec);
	while (ecx < arguments_from_argv->nbr_philos)
	{
		arguments_from_argv->philo_id++;
		chld_pid = fork();
		fork_handler(arguments_from_argv, chld_pid, ecx, launched_child_pids);
		launched_child_pids[ecx] = chld_pid;
		ecx++;
	}
	sem_post(&arguments_from_argv->sem_holder->sem_release_for_exec);
	wait_for_childs(launched_child_pids);
	return (ecx);
}

int init_sems(t_arg *argv_parsed)
{
	t_mysems	*ptr;

	ptr = argv_parsed->sem_holder;
	ptr->sem_release_for_exec = sem_open(SEM_MONITOR, O_CREAT, 0777, 1);
	if (ptr->sem_release_for_exec == SEM_FAILED)
		return (FAIL);
	ptr->sem_printf = sem_open(SEM_PRINTF, O_CREAT, 0777, 1);
	if (ptr->sem_printf == SEM_FAILED)
		return (sem_unlink(ptr->sem_release_for_exec), FAIL);
	ptr->sem_forks_queue = sem_open(SEM_FORKS, O_CREAT, 0777, 1);
	if (ptr->sem_forks_queue == SEM_FAILED)
		return (sem_unlink(ptr->sem_release_for_exec), sem_unlink(ptr->sem_printf), FAIL);
	return (SUCCESS);
}

int main(int argc, char *argv[])
{
	t_arg		*argv_parsed;
	pthread_t	*threads;
	
	if (check_program_arguments(argc, argv))
		return (FAIL);
	if (!ft_atoi(argv[1]))
		return (printf("Null simulation"),
				FAIL);
	if (init_arg(argv_parsed, argv) == FAIL)
		return (FAIL);
	if (init_sems(argv_parsed))
		return (free(argv_parsed->ate),
				free(argv_parsed->sem_holder),
				free(argv_parsed),
				FAIL);
	launch_processes(argv_parsed);
	return (ft_atoi(argv[1]),
			SUCCESS);
}