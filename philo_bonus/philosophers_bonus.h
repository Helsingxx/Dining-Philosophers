/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 15:01:07 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/20 23:16:15 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# define SEM_MONITOR "/monitorf8485cbf8b4390ca9ec8e0f6633d0fdbf422ed67e17a7fa4890227cc"
# define SEM_PRINTF "/printfae997cabd934749a146be50b43f50f258505c6b0f0ab79122b924f08"
# define SEM_FORKS "/forks5a6880bc9434f2225b8e7752cd2265339f59a2aa3dea537095a391da"
# define THREAD_FAIL_CREATE 101
# define FAIL 				1
# define SUCCESS			0

typedef struct s_arg
{	
	int				philo_id;
	int				end;
	char			**argv;
	
	int				nbr_philos;
	int				timetodie;
	int				timetoeat;
	int				timetosleep;
	int				gtimestamp;
	int				*times;
	int				*forks;

	int 			nbrtoeat;
	int				*ate;
	int				*time;
	int				exit;
	int				finish;
	int				to_be_eaten;
	
	int				is_gtime_set;
	struct timeval gtime;
	struct timeval gtime2;
	struct s_arg 	*arg;

	struct s_mysems		*sem_holder;
} t_arg;

typedef struct s_mysems
{
	sem_t sem_release_for_exec;
	sem_t sem_forks_queue;
	sem_t sem_printf;
}	t_mysems;

int	is_int(char *);
int	ft_atoi(const char *);
int	skip_int(char *);
void check_elapsed(int, int, t_arg *);
int 	eating(t_arg *, int, int);
int		thinking(t_arg *, int);
int		sleeping(t_arg *, int);
void	*ft_calloc(size_t, size_t);
int kill_him(t_arg *, int);
int	is_int0(char *);
int	ft_strlen(const char *);
void chad_wastetime(int);

#endif  