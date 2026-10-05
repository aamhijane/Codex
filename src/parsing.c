/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 21:50:53 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/24 00:24:19 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	validate_args(int argc, char **argv)
{
	char	*arg;
	int		i;

	arg = NULL;
	i = 1;
	while (i < argc - 1)
	{
		arg = ft_trim(argv[i]);
		if (*arg == '\0')
			return (log_error_msg(NUM_INVALID, arg, i - 1), -1);
		if (is_valid_number(arg) != 0)
			return (log_error_msg(NUM_INVALID, arg, i - 1), -1);
		if (is_num_overflow(arg) == 0)
			return (log_error_msg(NUM_LARGE, arg, i - 1), -1);
		if (i == 1 || i == 6)
		{
			if (atoi(argv[i]) == 0)
				return (log_error_msg(NUM_ZERO, arg, i - 1), -1);
		}
		i++;
	}
	arg = ft_trim(argv[i]);
	if (!(strcmp(arg, "edf") == 0 || strcmp(arg, "fifo") == 0))
		return (log_error_msg(SCHEDULER_INVALID, arg, i - 1), -1);
	return (0);
}

int	prepare_args(int argc, char **argv, t_args *args)
{
	char	*trim_scheduler;

	trim_scheduler = NULL;
	if (argc != 9)
	{
		fprintf(stderr, "Error: wrong number of arguments \
			(expected 8, got %d)\n", argc - 1);
		fprintf(stderr, "Usage: %s number_of_coders time_to_burnout time_to_compile \
			time_to_debug time_to_refactor number_of_compiles_required \
			dongle_cooldown scheduler\n", argv[0]);
		return (-1);
	}
	if (validate_args(argc, argv) != 0)
		return (-1);
	args->number_of_coders = atoi(argv[1]);
	args->time_to_burnout = atoi(argv[2]);
	args->time_to_compile = atoi(argv[3]);
	args->time_to_debug = atoi(argv[4]);
	args->time_to_refactor = atoi(argv[5]);
	args->number_of_compiles_required = atoi(argv[6]);
	args->dongle_cooldown = atoi(argv[7]);
	trim_scheduler = ft_trim(argv[8]);
	args->scheduler = malloc(strlen(trim_scheduler) + 1);
	if (args->scheduler == NULL)
		return (-1);
	ft_strcpy(args->scheduler, trim_scheduler);
	return (0);
}
