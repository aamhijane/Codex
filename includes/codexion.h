/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iot <ayamhija@student.1337.ma>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 21:50:56 by iot               #+#    #+#             */
/*   Updated: 2026/09/23 01:32:24 by iot              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

// ============================================================
// STANDARD LIBRARIES
// ============================================================

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <ctype.h>
# include <sys/time.h>

// ============================================================
// ENUMS
// ============================================================

//	errors
enum e_error_status
{
	ARG_LENGTH = 20,
	NUM_LARGE = 21,
	NUM_INVALID = 22,
	NUM_ZERO = 23,
	SCHEDULER_INVALID = 24
};

// ============================================================
// STRUCTURES
// ============================================================

//	args
typedef struct args
{
	int		number_of_coders;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	int		number_of_compiles_required;
	long	dongle_cooldown;
	char	*scheduler;
}		t_args;

// ============================================================
// FUNCTIONS DEFINITIONS
// ============================================================

//	args
int		validate_args(int argc, char **argv);
int		prepare_args(int argc, char **argv, t_args *cli_args);

//	errors
void	log_error_msg(enum e_error_status error_status, char *arg, int idx);

//	utils
int		is_valid_number(char *arg);
int		is_num_overflow(char *arg);
char	*ft_trim(char *str);
char	*ft_strcpy(char *dest, char *src);

#endif
