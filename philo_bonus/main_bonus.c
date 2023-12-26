/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:42:33 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/26 22:15:35 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	main(int argc, char *argv[])
{
	t_arg	*argv_parsed;

	if (check_program_arguments(argc, argv))
		return (EXIT_FAILURE);
	argv_parsed = (t_arg *) ft_calloc(sizeof(t_arg), 1);
	if (!argv_parsed)
		return (EXIT_FAILURE);
	if (init_arg(argv_parsed, argv) == EXIT_FAILURE)
		return (free(argv_parsed),
			EXIT_FAILURE);
	if (init_sems(argv_parsed) == PHIL_FAIL)
		return (free(argv_parsed->sem_holder),
			free(argv_parsed),
			EXIT_FAILURE);
	launch_processes(argv_parsed);
	return (EXIT_SUCCESS);
}
