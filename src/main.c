/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 21:51:16 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/24 01:08:45 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_args	args;
	t_sim	sim;

	if (prepare_args(argc, argv, &args) != 0)
		return (1);
	if (init_simulation(&sim, &args) != 0)
	{
		free(args.scheduler);
		return (1);
	}
	cleanup_simulation(&sim);
	free(args.scheduler);
	return (0);
}
