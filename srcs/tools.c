#include "malloc.h"

static void		*find_large(void *ptr)
{
	t_zone 		*tmp;

	tmp = g_mem->large;
	while (tmp)
	{
		if (((void*)tmp + sizeof(t_zone) + sizeof(t_alloc)) == ptr)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

static void		*find_by_type(t_zone *zone, size_t max_size, void *ptr)
{
	t_zone 		*tmp_zone;
	t_alloc		*mem;

	tmp_zone = zone;
	mem = NULL;
	while (tmp_zone)
	{
		if ((void*)ptr > (void*)tmp_zone && (void*)ptr < ((void*)tmp_zone + max_size))
		{
			return ((void*)ptr - sizeof(t_alloc));
		// 	mem = tmp_zone->start;
		// 	while (mem)
		// 	{
		// 		if (mem->data && (void*)mem->data == ptr && !mem->free)
		// 			return (mem);
		// 		mem = mem->next;
		// 	}
		}
		ft_print_mem(tmp_zone);
		ft_putchar('\n');
		ft_print_mem(tmp_zone->next);
		ft_putchar('\n');
		tmp_zone = tmp_zone->next;
	}
	return (NULL);
}

void			*find_alloc(void *ptr)
{
	void		*ret;

	ret = NULL;
	if ((ret = find_by_type(g_mem->tiny, TINY_ZONE_SIZE, ptr)) != NULL)
		return (ret);
	if ((ret = find_by_type(g_mem->small, SMALL_ZONE_SIZE, ptr)) != NULL)
		return (ret);
	if ((ret = find_large(ptr)) != NULL)
		return (ret);
	return (NULL);
}


void		*eco_search(size_t size, size_t type)
{
	t_zone	*zone;
	t_alloc	*mem;

	zone = !type ? g_mem->tiny : g_mem->small;
	mem = NULL;
	while (zone)
	{
		if (zone->size > (size + sizeof(t_alloc)))
		{
			mem = zone->start;
			while (mem)
			{
				if (mem->data && mem->free && (mem->size >= size))
				{
					mem->free = 0;
					return (mem);
				}
				if (!mem->master)
				{
					ft_print_mem(mem);
					ft_putstr("< mdeir\n");
				}
				if ((void *)mem->next && mem->next->next
					&& ((void *)mem->next->next < ((t_zone *)mem->master)->start
					|| (void *)mem->next->next > ((t_zone *)mem->master)->end))
				{
					ft_putstr("\n-----OLD--------\n");
					ft_putstr("\n -> mem = ");
					ft_print_mem(mem);
					ft_putstr("\n -> next = ");
					ft_print_mem(mem->next);
					ft_putstr("\n -> prev = ");
					ft_print_mem(mem->prev);
					ft_putstr("\n -> size = ");
					ft_print_mem((void *)mem->size);
					ft_putstr("\n -> free = ");
					ft_print_mem((void *)mem->free);
					ft_putstr("\n -> master = ");
					ft_print_mem(mem->master);
					ft_putstr("-> size = ");
					ft_print_mem(((t_zone*)mem->master)->end);
					ft_putstr("\n -> data = ");
					ft_print_mem(mem->data);
					ft_putstr("\n------NEW-------\n");
					ft_putstr("\n -> mem = ");
					ft_print_mem(mem->next);
					ft_putstr("\n -> next = ");
					ft_print_mem(mem->next->next);
					ft_putstr("\n -> prev = ");
					ft_print_mem(mem->next->prev);
					ft_putstr("\n -> size = ");
					ft_print_mem((void *)mem->next->size);
					ft_putstr("\n -> free = ");
					ft_print_mem((void *)mem->next->free);
					ft_putstr("\n -> master = ");
					ft_print_mem(mem->next->master);
					ft_putstr("-> size = ");
					ft_print_mem(((t_zone*)mem->next->master)->end);
					ft_putstr("\n -> data = ");
					ft_print_mem(mem->next->data);
					ft_putstr("\n-------END------\n");
				}
				mem = mem->next;
			}
		}
		zone = zone->next;
	}
	return (NULL);
}

void	*eco_alloc(size_t size, size_t type)
{
	void	*tmp;

	tmp = NULL;
	if (!type && size <= TINY_SIZE && g_mem->tiny && g_mem->tiny->last)
		tmp = eco_search(size, 0);
	else if (type && size <= SMALL_SIZE && g_mem->small && g_mem->small->last)
		tmp = eco_search(size, 1);
	return (tmp);
}