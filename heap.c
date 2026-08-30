/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 00:50:42 by adkhalil          #+#    #+#             */
/*   Updated: 2026/08/30 02:03:42 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_push(t_heap_node node, t_heap *heap)
{
	int			i;
	int			parent_i;
	t_heap_node	tmp;

	heap->nodes[heap->size] = node;
	heap->size++;
	i = heap->size - 1;
	parent_i = (i - 1) / 2;
	while (i > 0 && heap->nodes[i].priority < heap->nodes[parent_i].priority)
	{
		tmp = heap->nodes[i];
		heap->nodes[i] = heap->nodes[parent_i];
		heap->nodes[parent_i] = tmp;
		i = parent_i;
		parent_i = (i - 1) / 2;
	}
}

t_heap_node	heap_pop(t_heap *heap)
{
	t_heap_node	root;
	t_heap_node	tmp;
	int			i;
	int			left;
	int			right;
	int			smallest;

	root = heap->nodes[0];
	heap->nodes[0] = heap->nodes[heap->size - 1];
	heap->size--;
	i = 0;
	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < heap->size
			&& heap->nodes[left].priority < heap->nodes[smallest].priority)
			smallest = left;
		if (right < heap->size
			&& heap->nodes[right].priority < heap->nodes[smallest].priority)
			smallest = right;
		if (smallest == i)
			break ;
		tmp = heap->nodes[i];
		heap->nodes[i] = heap->nodes[smallest];
		heap->nodes[smallest] = tmp;
		i = smallest;
	}
	return (root);
}

t_heap	*heap_init(int capacity)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (!heap->nodes)
	{
		free(heap);
		return (NULL);
	}
	heap->size = 0;
	heap->capacity = capacity;
	return (heap);
}
