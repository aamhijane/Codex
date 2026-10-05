/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 23:54:12 by ayamhija          #+#    #+#             */
/*   Updated: 2026/10/02 23:56:33 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *h, int capacity)
{
	h->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (!h->nodes)
		return (-1);
	h->capacity = capacity;
	h->size = 0;
	return (0);
}

t_heap_node	heap_peek(t_heap *h)
{
	return (h->nodes[0]);
}

void	heap_free(t_heap *h)
{
	if (h->nodes)
		free(h->nodes);
	h->nodes = NULL;
	h->capacity = 0;
	h->size = 0;
}
