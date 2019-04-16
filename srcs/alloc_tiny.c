#include "malloc.h"

static void		*alloc_mem_tiny(size_t size)
{
	t_alloc 	*new;
	t_alloc		*last;

	if (!g_mem->tiny_last)
		return (NULL);
	last = g_mem->tiny_last->last ? g_mem->tiny_last->last : g_mem->tiny_last->start;
	if (g_mem->tiny_last->last)
		new = (void*)last + sizeof(t_alloc) + last->size;
	else
		new = (void*)last;
	ft_bzero(new, sizeof(t_alloc));
	new->size = size;
	new->data = (void*)new + sizeof(t_alloc);
	new->prev = g_mem->tiny_last->last ? g_mem->tiny_last->last : NULL;
	new->master = g_mem->tiny_last;
	if (g_mem->tiny_last->last)
		((t_alloc*)g_mem->tiny_last->last)->next = (void*)new;
	g_mem->tiny_last->size -= (size + sizeof(t_alloc));
	g_mem->tiny_last->last = (void*)new;
	return (new);
}

void			*alloc_tiny(size_t size)
{
	t_alloc 	*new_tiny;
	t_alloc		*last;

	new_tiny = NULL;
	if (!g_mem->tiny)
	{
		g_mem->tiny = alloc_zone(TINY_ZONE_SIZE);
		g_mem->tiny_last = g_mem->tiny;
	}
	last = g_mem->tiny_last->last ? g_mem->tiny_last->last : g_mem->tiny_last->start;
	if ((new_tiny = eco_alloc(size, 0)) != NULL)
	{
		ft_putstr("Next one is from EKOLOL > ");
		return (new_tiny->data);
	}
	// verifier plutot si il reste de la place dans une des zones
	if ((void*)last + sizeof(t_alloc) + last->size > ((void*)g_mem->tiny_last->end - (size + sizeof(t_alloc))))
	{
		((t_zone*)g_mem->tiny_last)->next = (t_zone*)alloc_zone(TINY_ZONE_SIZE);
		g_mem->tiny_last = ((t_zone*)g_mem->tiny_last)->next;
	}
	new_tiny = alloc_mem_tiny(size);
	return (new_tiny ? (void*)new_tiny->data : NULL);
}