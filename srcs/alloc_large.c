/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_large.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlistrat <vlistrat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 13:14:26 by vlistrat          #+#    #+#             */
/*   Updated: 2019/05/06 13:15:53 by vlistrat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void		*alloc_mem_large(size_t size)
{
	t_alloc		*new;

	if (!g_mem->large_last)
		return (NULL);
	new = (void*)g_mem->large_last + sizeof(t_zone);
	bzero(new, sizeof(t_alloc));
	new->size = size;
	new->data = (void*)new + sizeof(t_alloc);
	new->prev = g_mem->large_last->last ? g_mem->large_last->last : NULL;
	new->master = g_mem->large_last;
	g_mem->large_last->next_safe = NULL;
	if (g_mem->large_last->last != NULL)
		((t_alloc*)g_mem->large_last->last)->next = new;
	g_mem->large_last->last = new;
	return (new);
}

void			*alloc_large(size_t size)
{
	t_alloc		*new_large;

	new_large = NULL;
	if (!g_mem->large)
	{
		if (!(g_mem->large = alloc_zone(size + sizeof(t_alloc) + \
						sizeof(t_zone))))
			return (NULL);
		g_mem->large_last = g_mem->large;
	}
	else
	{
		if (!(g_mem->large_last->next = alloc_zone(size + sizeof(t_alloc) + \
						sizeof(t_zone))))
			return (NULL);
		g_mem->large_last = g_mem->large_last->next;
	}
	new_large = alloc_mem_large(size);
	return (new_large ? (void*)new_large->data : NULL);
}
