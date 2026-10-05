/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 21:50:23 by ayamhija          #+#    #+#             */
/*   Updated: 2026/09/24 20:41:38 by ayamhija         ###   ########.fr       */
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
typedef struct s_args
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

//	dongle
typedef struct s_dongle
{
	int				is_free;
	long			last_release_time;
	pthread_cond_t	cond;
	pthread_mutex_t	lock;
}	t_dongle;

//	coder
typedef struct s_coder
{
	int				id;
	int				compile_count;
	long			last_compile_time;
	t_dongle		*first;
	t_dongle		*second;
	struct s_sim	*sim;
	pthread_mutex_t	lock;
}	t_coder;

//	simulation
typedef struct s_sim
{
	int				is_over;
	long			start_time;
	t_args			*args;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_mutex_t	sim_lock;
	pthread_mutex_t	log_lock;
}	t_sim;

// ============================================================
// FUNCTIONS DEFINITIONS
// ============================================================

//	args
int		validate_args(int argc, char **argv);
int		prepare_args(int argc, char **argv, t_args *args);

//	errors
void	log_error_msg(enum e_error_status error_status, char *arg, int idx);

//	utils
int		is_valid_number(char *arg);
int		is_num_overflow(char *arg);
char	*ft_trim(char *str);
char	*ft_strcpy(char *dest, char *src);

//	simulation
int		init_simulation(t_sim *sim, t_args *args);
void	cleanup_simulation(t_sim *sim);
void	print_simulation(t_sim *sim);

//	memory allocation
int		allocate_coders(t_sim *sim);
int		allocate_dongles(t_sim *sim);
void	free_coders(t_sim *sim);
void	free_dongles(t_sim *sim);

#endif
