/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_zone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlistrat <vlistrat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 13:16:12 by vlistrat          #+#    #+#             */
/*   Updated: 2019/05/06 13:16:14 by vlistrat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_zone	*alloc_zone(size_t size)
{
	t_zone		*new;

	new = NULL;
	size = align_number(size, 4096);
	if ((new = mmap(0, size, PROT, FLAGS, -1, 0)) == MAP_FAILED)
		return (NULL);
	bzero(new, (sizeof(t_zone) + sizeof(t_alloc)));
	new->start = (void*)new + sizeof(t_zone);
	new->end = (void*)new + size;
	new->last = NULL;
	new->next_safe = (void*)new->start;
	new->next = NULL;
	new->size = size - sizeof(t_zone);
	return (new);
}
