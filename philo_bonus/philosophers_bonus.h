/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 15:01:07 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/26 22:27:07 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <pthread.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <unistd.h>
# include <signal.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# define PHIL_NAME_MONITOR "/monitorf8485cbf8b4390caaaab"
# define PHIL_NAME_PRINTF "/printfae997cabd934749a14aaab"
# define PHIL_NAME_FORKS "/forks5a6880bc9434f2225b8eaaab"
# define PHIL_THREAD_FAIL_CREATE	101
# define PHIL_THREAD_MUTEX_FAIL		111
# define PHIL_ABORT_PROCESS			151
# define PHIL_FAIL 					1
# define PHIL_SUCCESS				0
# define PHIL_DONE					1
# define PHIL_LAP					0
# define PHIL_KILLED				1
# define PHIL_DIED					1
# define PHIL_FINISHED				200

typedef struct s_arg
{
	int				philo_id;
	int				end;
	char			**argv;

	int				nbr_philos;
	int				timetodie;
	int				timetoeat;
	int				timetosleep;

	int				ate;
	int				to_be_eaten;

	time_t			lasttimeate;
	struct timeval	gtime;
	struct timeval	gtime2;
	struct s_arg	*arg;

	struct s_mysems	*sem_holder;

	pthread_mutex_t	mut_printf;
	pthread_mutex_t	mut_freeuse;
}	t_arg;

typedef struct s_mysems
{
	sem_t	*sem_release_for_exec;
	sem_t	*sem_forks_queue;
	sem_t	*sem_printf;
}	t_mysems;

int		ft_atoi(const char *str);
int		is_int0(char *arg);
void	*ft_calloc(size_t count, size_t size);

int		attempt_kill_philosopher(t_arg *arguments_from_argv,
			int current_test_iteration);
void	take_otherfork_starteating(t_arg *arguments_from_argv);
int		eating(t_arg *arguments_from_argv);
void	sleeping(t_arg *arguments_from_argv);
void	thinking(t_arg *arguments_from_argv);
void	chad_wastetime(int span_in_milliseconds);

void	*philosopher_normal_routine(void *arguments_from_argv);
void	*monitor_death_time(void *arguments_from_argv);
int		launch_philosopher_and_monitor(t_arg *arguments_from_argv);

int		check_program_arguments(int argc, char *argv[]);
int		init_arg(t_arg *arg, char **argv);
int		init_sems(t_arg *argv_parsed);
void	unlink_all_semaphores(void);

void	failure_create_process(int ecx, int *launched_child_pids);
void	kill_rest(int *launched_child_pids, int excepted_child,
			int number_of_targets);
void	wait_for_childs(int *launched_child_pids, int nbr_philos);
void	fork_handler(t_arg	*arguments_from_argv, pid_t chld_pid,
			int ecx, int *launched_child_pids);
int		launch_processes(t_arg *arguments_from_argv);

#endif  