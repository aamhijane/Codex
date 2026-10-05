/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 21:50:23 by ayamhija          #+#    #+#             */
/*   Updated: 2026/10/03 00:11:28 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

// ============================================================
// STANDARD LIBRARIES
// ============================================================

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <ctype.h>
# include <time.h>

// ============================================================
// ENUMS
// ============================================================

//	coder state
enum e_coder_state
{
	IDLE,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	BURNOUT
};

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

//	heap queue
typedef struct s_heap_node
{
	int		coder_id;
	long	key;
	long	seq;
}		t_heap_node;

typedef struct s_heap
{
	t_heap_node	*nodes;
	int			size;
	int			capacity;
}		t_heap;

//	dongle
typedef struct s_dongle
{
	int				is_free;
	long			last_release_time;
	t_heap			queue;
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
	pthread_t		thread;
	pthread_mutex_t	lock;
}	t_coder;

//	simulation
typedef struct s_sim
{
	int				is_running;
	long			start_time;
	long			next_seq;
	t_args			*args;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		monitor;
	pthread_mutex_t	sim_lock;
	pthread_mutex_t	log_lock;
}	t_sim;

// ============================================================
// FUNCTIONS DEFINITIONS
// ============================================================

//	args
int			validate_args(int argc, char **argv);
int			prepare_args(int argc, char **argv, t_args *args);

//	errors
void		log_error_msg(enum e_error_status error_status, char *arg, int idx);

//	utils
int			is_valid_number(char *arg);
int			is_num_overflow(char *arg);
char		*ft_trim(char *str);
char		*ft_strcpy(char *dest, char *src);
long		get_time_in_ms(void);

//	simulation
int			init_simulation(t_sim *sim, t_args *args);
void		cleanup_simulation(t_sim *sim);
void		print_simulation(t_sim *sim);

//	memory allocation
int			allocate_coders(t_sim *sim);
int			allocate_dongles(t_sim *sim);
void		free_coders(t_sim *sim);
void		free_dongles(t_sim *sim);

//	logs
void		log_state(t_coder *coder, enum e_coder_state state);

//	coder
int			coders_compiles_count(t_sim *sim, int idx);
void		*coder_routine(void *arg);

//	min-heap queue

int			heap_push(t_heap *h, t_heap_node n);
int			heap_init(t_heap *h, int capacity);
t_heap_node	heap_peek(t_heap *h);
t_heap_node	heap_pop(t_heap *h);
void		heap_free(t_heap *h);

//	dongle
int			take_dongle(t_sim *sim, t_dongle *dongle);
void		release_dongle(t_dongle *dongle);
void		broadcast_all(t_sim *sim);

//	monitor
int			is_sim_over(t_sim *sim);
void		*monitor_routine(void *arg);

#endif
