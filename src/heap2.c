/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamhija <ayamhija@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 23:54:44 by ayamhija          #+#    #+#             */
/*   Updated: 2026/10/03 00:22:33 by ayamhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	node_less(t_heap_node a, t_heap_node b)
{
	if (a.key < b.key)
		return (1);
	if (a.key == b.key && a.seq < b.seq)
		return (1);
	return (0);
}

void	bubble_up(t_heap *h, int idx)
{
	int			i;
	int			p;
	t_heap_node	tmp;

	i = idx;
	while (i > 0)
	{
		p = (i - 1) / 2;
		if (node_less(h->nodes[i], h->nodes[p]))
		{
			tmp = h->nodes[i];
			h->nodes[i] = h->nodes[p];
			h->nodes[p] = tmp;
			i = p;
		}
		else
			return ;
	}
}

void	bubble_down(t_heap *h, int idx)
{
	int			i;
	int			smallest;
	int			left;
	int			right;
	t_heap_node	tmp;

	i = idx;
	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if ((left < h->size) && node_less(h->nodes[left], h->nodes[smallest]))
			smallest = left;
		if ((right < h->size) && node_less(h->nodes[right], h->nodes[smallest]))
			smallest = right;
		if (smallest == i)
			return ;
		tmp = h->nodes[i];
		h->nodes[i] = h->nodes[smallest];
		h->nodes[smallest] = tmp;
		i = smallest;
	}
}

int	heap_push(t_heap *h, t_heap_node n)
{
	if (h->size >= h->capacity)
		return (-1);
	h->nodes[h->size] = n;
	h->size++;
	bubble_up(h, h->size - 1);
	return (0);
}

t_heap_node	heap_pop(t_heap *h)
{
	t_heap_node	top;

	top = h->nodes[0];
	h->nodes[0] = h->nodes[h->size - 1];
	h->size--;
	if (h->size > 0)
		bubble_down(h, 0);
	return (top);
}
