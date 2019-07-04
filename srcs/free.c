/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlistrat <vlistrat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 13:28:58 by vlistrat          #+#    #+#             */
/*   Updated: 2019/07/04 14:57:57 by vlistrat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void			free_large(void *mem)
{
	t_zone			*tmp;
	t_zone			*next;

	tmp = g_mem->large ? g_mem->large : NULL;
	next = NULL;
	if (!tmp)
		return ;
	while (tmp && tmp->next)
	{
		if (mem == tmp->next)
		{
			next = tmp->next->next;
			if (tmp->next == g_mem->large_last)
			{
				g_mem->large_last = tmp;
				g_mem->large_last->last = (void*)tmp + sizeof(t_zone);
			}
			if (munmap(tmp->next, ((t_zone*)tmp->next)->size + sizeof(t_zone)))
				ft_putendl_fd("Can't unmap memory zone", 2);
			tmp->next = next;
			return ;
		}
		tmp = tmp->next;
	}
}

static size_t		check_no_alloc(t_alloc *mem)
{
	t_zone			*zone;
	t_alloc			*tmp;

	zone = mem->master;
	tmp = zone->start;
	while (tmp)
	{
		if (!tmp->free)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

static void			free_zone(t_alloc *mem)
{
	t_zone			*zone;
	t_zone			*tmp;

	zone = mem->master;
	tmp = mem->size <= TINY_SIZE ? g_mem->tiny : g_mem->small;
	while (tmp && tmp->next)
	{
		if (tmp->next == zone)
		{
			if (tmp->next == g_mem->tiny_last)
			{
				g_mem->tiny_last = tmp;
				g_mem->tiny_last->last = find_last_in_zone(tmp);
			}
			else if (tmp->next == g_mem->small_last)
			{
				g_mem->small_last = tmp;
				g_mem->small_last->last = find_last_in_zone(tmp);
			}
			tmp->next = tmp->next->next;
		}
		tmp = tmp->next;
	}
	if (munmap(zone, zone->size + sizeof(t_zone)))
		ft_putendl_fd("Can't unmap memory zone", 2);
}

static void			free_normal(void *mem)
{
	t_alloc			*tmp;

	tmp = mem;
	((t_zone*)tmp->master)->size += tmp->size + sizeof(t_alloc);
	tmp->free = 1;
	if ((((t_zone*)tmp->master)->size == align_number(TINY_ZONE_SIZE, 4096) - \
				sizeof(t_zone) || ((t_zone*)tmp->master)->size == \
				align_number(SMALL_ZONE_SIZE, 4096) - sizeof(t_zone)) \
				&& check_no_alloc(tmp))
		free_zone(tmp);
}

void				free(void *ptr)
{
	void			*tmp;

	tmp = NULL;
	if (!ptr)
		return ;
	if ((tmp = find_alloc(ptr)))
		free_normal(tmp);
	else if ((tmp = find_large(ptr)))
		free_large(tmp);
	else
	{
		ft_print_mem(ptr);
		ft_putendl_fd(" : ptr not allocated.", 2);
	}
}
