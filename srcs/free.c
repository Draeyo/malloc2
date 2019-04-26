#include "malloc.h"

static void		free_large(void *mem)
{
	t_zone	*tmp;
	//t_alloc	*tmp_mem;

	tmp = g_mem->large ? g_mem->large : NULL;
	//tmp_mem = NULL;
	if (!tmp)
		return ;
	while (tmp && tmp->next)
	{
	//	tmp_mem = (void*)tmp->next + sizeof(t_zone);
		//if (mem == (void*)tmp_mem->data)
		if (mem == tmp->next)
		{
			// if (tmp == g_mem->large_last && tmp != g_mem->large)
			// {
			// 	// A VERIFIER
			// 	g_mem->large_last = tmp;
			// 	g_mem->large_last->last = (void*)tmp + sizeof(t_zone);
			// }
			if (tmp->next == g_mem->large_last)
			{
				g_mem->large_last = tmp;
				g_mem->large_last->last = (void*)tmp + sizeof(t_zone);
			}
			tmp->next = tmp->next->next;
			if (munmap(tmp->next, tmp->size + sizeof(t_zone)))
				ft_putendl_fd("Can't unmap memory zone", 2);
			// ft_print_mem(mem);
			// ft_putstr(" <- FREE LARGE\n");
			return ;
		}
		tmp = tmp->next;
	}
}

static size_t		check_no_alloc(t_alloc *mem)
{
	t_zone 		*zone;
	t_alloc 	*tmp;

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

static void 	free_zone(t_alloc *mem)
{
	t_zone		*zone;

	zone = mem->master;
	if (munmap(zone, zone->size + sizeof(t_zone)))
		ft_putendl_fd("Can't unmap memory zone", 2);
}

static void 	free_normal(void *mem)
{
	t_alloc		*tmp;

	tmp = mem;
	((t_zone*)tmp->master)->size += tmp->size + sizeof(t_alloc);
	tmp->free = 1;
	if ((((t_zone*)tmp->master)->size == TINY_ZONE_SIZE || ((t_zone*)tmp->master)->size == SMALL_ZONE_SIZE) && check_no_alloc(tmp))
		free_zone(tmp);
}

void		free(void *ptr)
{
	void	*tmp;

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
