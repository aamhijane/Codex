/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 21:50:37 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/23 21:50:42 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_error_msg(enum e_error_status error_status, char *arg, int idx)
{
	char	*args_names[8];

	args_names[0] = "number_of_coders";
	args_names[1] = "time_to_burnout";
	args_names[2] = "time_to_compile";
	args_names[3] = "time_to_debug";
	args_names[4] = "time_to_refactor";
	args_names[5] = "number_of_compiles_required";
	args_names[6] = "dongle_cooldown";
	args_names[7] = "scheduler";
	if (error_status == NUM_INVALID)
		fprintf(stderr,
			"Error: invalid '%s', must be a positive number\n",
			arg);
	else if (error_status == NUM_LARGE)
		fprintf(stderr, "Error: '%s' is out of range (max: 2147483647)\n", arg);
	else if (error_status == NUM_ZERO)
		fprintf(stderr,
			"ERROR: '%s' cannot be zero, need at least 1\n",
			args_names[idx]);
	else if (error_status == SCHEDULER_INVALID)
		fprintf(stderr, "ERROR: scheduler must be 'fifo' or 'edf'\n");
}
