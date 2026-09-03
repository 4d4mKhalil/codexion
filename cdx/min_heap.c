/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkhalil <adkhalil@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 00:50:42 by adkhalil          #+#    #+#             */
/*   Updated: 2026/09/03 00:07:10 by adkhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_push(t_heap_node node, t_heap *heap)
{
	int			node_i;
	int			parent_i;
	t_heap_node	tmp;

	heap->nodes[heap->size] = node;
	heap->size++;
	node_i = heap->size - 1;
	parent_i = (node_i - 1) / 2;
	while (node_i > 0
		&& heap->nodes[node_i].priority < heap->nodes[parent_i].priority)
	{
		tmp = heap->nodes[node_i];
		heap->nodes[node_i] = heap->nodes[parent_i];
		heap->nodes[parent_i] = tmp;
		node_i = parent_i;
		parent_i = (node_i - 1) / 2;
	}
}

static void	bubble_down(t_heap *heap)
{
	t_heap_node	tmp;
	int			node_i;
	int			smallest_i;

	node_i = 0;
	while (1)
	{
		smallest_i = node_i;
		if (2 * node_i + 1 < heap->size && heap->nodes[2 * node_i + 1].priority
			< heap->nodes[smallest_i].priority)
			smallest_i = 2 * node_i + 1;
		if (2 * node_i + 2 < heap->size && heap->nodes[2 * node_i + 2].priority
			< heap->nodes[smallest_i].priority)
			smallest_i = 2 * node_i + 2;
		if (smallest_i == node_i)
			break ;
		tmp = heap->nodes[node_i];
		heap->nodes[node_i] = heap->nodes[smallest_i];
		heap->nodes[smallest_i] = tmp;
		node_i = smallest_i;
	}
}

t_heap_node	heap_pop(t_heap *heap)
{
	t_heap_node	root;

	root = heap->nodes[0];
	heap->nodes[0] = heap->nodes[heap->size - 1];
	heap->size--;
	bubble_down(heap);
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
