/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_small.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlistrat <vlistrat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 13:16:23 by vlistrat          #+#    #+#             */
/*   Updated: 2019/05/06 13:21:31 by vlistrat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void		*alloc_mem_small(size_t size)
{
	t_alloc		*new;
	t_alloc		*last;

	if (!g_mem->small_last)
		return (NULL);
	last = g_mem->small_last->last ? g_mem->small_last->last \
		: g_mem->small_last->start;
	if (g_mem->small_last->last)
		new = (void*)last + sizeof(t_alloc) + last->size;
	else
		new = (void*)last;
	ft_bzero(new, sizeof(t_alloc));
	new->size = size;
	new->data = (void*)new + sizeof(t_alloc);
	new->prev = g_mem->small_last->last ? g_mem->small_last->last : NULL;
	new->master = g_mem->small_last;
	if (g_mem->small_last->last)
		((t_alloc*)g_mem->small_last->last)->next = new;
	g_mem->small_last->size -= (size + sizeof(t_alloc));
	g_mem->small_last->last = new;
	return (new);
}

void			*alloc_small(size_t size)
{
	t_alloc		*new_small;
	t_alloc		*last;

	new_small = NULL;
	if (!g_mem->small)
	{
		g_mem->small = alloc_zone(SMALL_ZONE_SIZE);
		g_mem->small_last = g_mem->small;
	}
	last = g_mem->small_last->last ? g_mem->small_last->last \
		: g_mem->small_last->start;
	if ((new_small = eco_alloc(size, 1)) != NULL)
		return (new_small->data);
	if (((void*)last->data + last->size + sizeof(t_alloc) + size) > \
			(void*)g_mem->small_last->end)
	{
		((t_zone*)g_mem->small_last)->next = \
			(t_zone*)alloc_zone(SMALL_ZONE_SIZE);
		g_mem->small_last = ((t_zone*)g_mem->small_last)->next;
	}
	new_small = alloc_mem_small(size);
	return (new_small ? (void*)new_small->data : NULL);
}
