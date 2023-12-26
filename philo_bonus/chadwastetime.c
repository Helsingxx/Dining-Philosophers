/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chadwastetime.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 22:24:49 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/26 22:26:44 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	chad_wastetime(int span_in_milliseconds)
{
	struct timeval	time_now;
	struct timeval	initial_time;

	gettimeofday(&initial_time, NULL);
	gettimeofday(&time_now, NULL);
	while ((time_now.tv_sec - initial_time.tv_sec) * 1000
		+ (time_now.tv_usec - initial_time.tv_usec) / 1000
		< span_in_milliseconds)
	{
		usleep(10);
		gettimeofday(&time_now, NULL);
	}
}
