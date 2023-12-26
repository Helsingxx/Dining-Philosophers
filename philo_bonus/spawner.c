/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 18:02:42 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/26 22:19:24 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	failure_create_process(int ecx, int *launched_child_pids)
{
	int	loop;

	loop = 0;
	while (loop < ecx)
		kill(launched_child_pids[loop++], SIGTERM);
}

void	kill_rest(int *launched_child_pids, int excepted_child,
					int number_of_targets)
{
	int	ecx;

	ecx = 0;
	while (ecx < number_of_targets)
	{
		if (launched_child_pids[ecx] != excepted_child)
			kill(launched_child_pids[ecx], SIGTERM);
		ecx++;
	}
}

void	wait_for_childs(int *launched_child_pids, int nbr_philos)
{
	int		exit_status;
	pid_t	return_pid;

	return_pid = waitpid(-1, &exit_status, 0);
	while (return_pid != -1)
	{
		if ((WIFEXITED(exit_status) && WEXITSTATUS(exit_status)
				!= PHIL_FINISHED) || WIFSIGNALED(exit_status))
		{
			kill_rest(launched_child_pids, return_pid, nbr_philos);
			return ;
		}
		return_pid = waitpid(-1, &exit_status, 0);
	}
}

void	fork_handler(t_arg	*arguments_from_argv,
						pid_t chld_pid, int ecx, int *launched_child_pids)
{
	t_arg	*ptr;

	ptr = arguments_from_argv;
	if (chld_pid == -1)
		failure_create_process(ecx, launched_child_pids);
	if (!chld_pid)
	{
		if (launch_philosopher_and_monitor(arguments_from_argv)
			== PHIL_THREAD_FAIL_CREATE)
			exit(PHIL_ABORT_PROCESS);
	}
}

int	launch_processes(t_arg *arguments_from_argv)
{
	int		*launched_child_pids;
	pid_t	chld_pid;
	int		ecx;

	launched_child_pids = (int *) ft_calloc(sizeof(int),
			arguments_from_argv->nbr_philos);
	ecx = 0;
	chld_pid = -1;
	gettimeofday(&arguments_from_argv->gtime, NULL);
	while (ecx < arguments_from_argv->nbr_philos)
	{
		arguments_from_argv->philo_id++;
		chld_pid = fork();
		fork_handler(arguments_from_argv, chld_pid, ecx, launched_child_pids);
		launched_child_pids[ecx] = chld_pid;
		ecx++;
	}
	wait_for_childs(launched_child_pids, arguments_from_argv->nbr_philos);
	return (ecx);
}
