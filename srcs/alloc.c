#include "malloc.h"

static t_zone	*alloc_zone(size_t size)
{
	t_zone		*new;

	new = NULL;
	size = align_number(size, 4096);
	if ((new = mmap(0, size, PROT, FLAGS, -1, 0)) == MAP_FAILED)
		return (NULL);
	ft_bzero(new, sizeof(t_zone));
	new->start = (void*)new + sizeof(t_zone);
	ft_bzero(new->start, sizeof(t_alloc));
	new->end = (void*)new + size;
	new->last = NULL;
	new->next_safe = (void*)new->start;
	new->next = NULL;
	new->size = size - sizeof(t_zone);
	return (new);
}

static void		*alloc_mem_tiny(size_t size)
{
	t_alloc 	*new;
	t_alloc		*last;

	if (!g_mem->tiny_last)
		return (NULL);
	last = g_mem->tiny_last->last ? g_mem->tiny_last->last : g_mem->tiny_last->start;
	if (g_mem->tiny_last->last)
		new = (void*)last->data + last->size;
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
	g_mem->tiny_last->last = new;
	return (new);
}

static void		*alloc_mem_small(size_t size)
{
	t_alloc 	*new;
	t_alloc		*last;

	if (!g_mem->small_last)
		return (NULL);
	last = g_mem->small_last->last ? g_mem->small_last->last : g_mem->small_last->start;
	if (g_mem->small_last->last)
		new = (void*)last->data + last->size;
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

static void		*alloc_mem_large(size_t size)
{
	t_alloc 	*new;

	if (!g_mem->large_last)
		return (NULL);
	new = (void*)g_mem->large_last + sizeof(t_zone);
	ft_bzero(new, sizeof(t_alloc));
	new->size = size;
	new->data = (void*)new + sizeof(t_alloc);
	new->master = g_mem->large_last;
	g_mem->large_last->next_safe = NULL;
	if (g_mem->large_last->last != NULL)
		((t_alloc*)g_mem->large_last->last)->next = new;
	g_mem->large_last->last = new;
	return (new);
}

void			*alloc_tiny(size_t size)
{
	t_alloc 	*new_tiny;
	t_alloc		*last;

	if (!g_mem->tiny)
	{
		g_mem->tiny = alloc_zone(TINY_ZONE_SIZE);
		g_mem->tiny_last = g_mem->tiny;
	}
	last = g_mem->tiny_last->last ? g_mem->tiny_last->last : g_mem->tiny_last->start;
	if ((new_tiny = eco_alloc(size, 0)) != NULL)
		return (new_tiny->data);
	// verifier plutot si il reste de la place dans une des zones
	if ((void*)last->data + last->size + size + sizeof(t_alloc) > g_mem->tiny_last->end)
	{
		g_mem->tiny_last->next = alloc_zone(TINY_ZONE_SIZE);
		g_mem->tiny_last = g_mem->tiny_last->next;
	}
	new_tiny = alloc_mem_tiny(size);
	return (new_tiny ? (void*)new_tiny->data : NULL);
}

void	*alloc_small(size_t size)
{
	t_alloc 	*new_small;
	t_alloc		*last;

	if (!g_mem->small)
	{
		g_mem->small = alloc_zone(SMALL_ZONE_SIZE);
		g_mem->small_last = g_mem->small;
	}
	last = g_mem->small_last->last ? g_mem->small_last->last : g_mem->small_last->start;
	if ((new_small = eco_alloc(size, 1)) != NULL)
		return (new_small->data);
	// verifier plutot si il reste de la place dans une des zones
	// if ((void*)g_mem->small_last->last + size + sizeof(t_alloc) >= g_mem->small_last->end)
	if ((void*)last->data + last->size + size + sizeof(t_alloc) > g_mem->small_last->end)
	{
		g_mem->small_last->next = alloc_zone(SMALL_ZONE_SIZE);
		g_mem->small_last = g_mem->small_last->next;
	}
	new_small = alloc_mem_small(size);
	return (new_small ? (void*)new_small->data : NULL);

}

void	*alloc_large(size_t size)
{
	t_alloc		*new_large;

	new_large = NULL;
	if (!g_mem->large)
	{
		g_mem->large = alloc_zone(size + sizeof(t_alloc) + sizeof(t_zone));
		g_mem->large_last = g_mem->large;
	}
	else
	{
		g_mem->large_last->next = alloc_zone(size + sizeof(t_alloc) + sizeof(t_zone));
		g_mem->large_last = g_mem->large_last->next;
	}
	new_large = alloc_mem_large(size);
	return (new_large ? (void*)new_large->data : NULL);
}