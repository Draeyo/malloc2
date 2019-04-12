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
	t_zone 		*tmp;
	t_alloc		*mem;

	tmp = zone;
	mem = NULL;
	while (tmp)
	{
		if ((void*)ptr > (void*)tmp && (void*)ptr < ((void*)tmp + max_size))
		{
			mem = tmp->start;
			while (mem)
			{
				if (mem->data && (void*)mem->data == ptr && !mem->free)
					return (mem);
				mem = mem->next;
			}
		}
		tmp = tmp->next;
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

// static void	func_mdr(void *ptr)
// {
// 	t_alloc *alloc_ptr;

// 	if (!g_mem || !g_mem->tiny || !g_mem->tiny->start)
// 		return ;
// 	if (ptr < (void *)g_mem || ptr > g_mem->tiny->end)
// 	{
// 		ft_putstr("start : ");
// 		ft_print_mem((void*)g_mem);
// 		ft_putstr(" - end : ");
// 		ft_print_mem((void*)g_mem->tiny->end);
// 		ft_putstr("\n");
// 		ft_print_mem(ptr);
// 		ft_putstr(" => CMON BRU\n");
// 	}
// 	alloc_ptr = g_mem->tiny->start;
// 	while (alloc_ptr)
// 	{
// 		if (ptr != alloc_ptr
// 			&& (ptr >= alloc_ptr->data && ptr < alloc_ptr->data + alloc_ptr->size))
// 		{
// 			ft_putendl("BAD NEXT");
// 			ft_putstr("bad ptr : ");
// 			ft_print_mem(ptr);
// 			ft_putstr(" - ");
// 			ft_putstr("victim ptr : ");
// 			ft_print_mem(alloc_ptr);
// 			ft_putstr("\n");
// 			ft_print_mem(alloc_ptr);
// 	 		ft_putstr("<== alloc_ptr\n");
// 	 		ft_print_mem((void*)alloc_ptr->next);
// 	 		ft_putstr("<== next\n");
// 	 		func_mdr(alloc_ptr->next);
// 	 		ft_print_mem((void*)alloc_ptr->prev);
// 	 		ft_putstr("<== prev\n");
// 	 		ft_print_mem((void*)alloc_ptr->data);
// 	 		ft_putstr("<== data\n");
// 	 		ft_print_mem((void*)alloc_ptr->size);
// 	 		ft_putstr("<== size\n");
// 	 		ft_print_mem((void*)alloc_ptr->free);
// 	 		ft_putstr("<== free\n");
// 	 		ft_print_mem((void*)alloc_ptr->padding);
// 	 		ft_putstr("<== padding\n");
// 		}
// 		alloc_ptr = alloc_ptr->next;
// 	}
// }

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
	if (!type && size <= TINY_SIZE && g_mem->tiny)
		tmp = eco_search(size, 0);
	else if (type && size <= SMALL_SIZE && g_mem->small)
		tmp = eco_search(size, 1);
	return (tmp);
}