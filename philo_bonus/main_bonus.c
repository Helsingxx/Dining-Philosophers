/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:42:33 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/21 18:19:06 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int main(int argc, char *argv[])
{
	t_arg		*argv_parsed;
	
	argv_parsed = (t_arg *) ft_calloc(sizeof(t_arg), 1);
	if (!argv_parsed)
		return (PHIL_FAIL);
	if (check_program_arguments(argc, argv))
		return (PHIL_FAIL);
	if (!ft_atoi(argv[1]))
		return (printf("Null simulation"),
				PHIL_FAIL);
	if (init_arg(argv_parsed, argv) == PHIL_FAIL)
		return (PHIL_FAIL);
	if (init_sems(argv_parsed))
		return (free(argv_parsed->sem_holder),
				free(argv_parsed),
				PHIL_FAIL);
	launch_processes(argv_parsed);
	return (PHIL_SUCCESS);
}