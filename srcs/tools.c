#include "malloc.h"

static void		*find_large(void *ptr)
{
	t_zone 		*tmp;

	tmp = g_mem->large;
	while (tmp)
	{
		if (((t_alloc*)tmp->start)->data == ptr)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

static void		*find_by_type(t_zone *zone, void *ptr)
{
	t_zone 		*tmp_zone;

	tmp_zone = zone;
	while (tmp_zone)
	{
		if ((void*)ptr > (void*)tmp_zone && (void*)ptr < (void*)tmp_zone->end)
			return ((void*)ptr - sizeof(t_alloc));
		tmp_zone = tmp_zone->next;
	}
	return (NULL);
}

void			*find_alloc(void *ptr)
{
	void		*ret;

	ret = NULL;
	if ((ret = find_by_type(g_mem->tiny, ptr)) != NULL)
		return (ret);
	if ((ret = find_by_type(g_mem->small, ptr)) != NULL)
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
	while (zone)
	{
		if (zone->size > (size + sizeof(t_alloc)))
		{
			mem = zone->start;
			while (mem)
			{
				if (mem->free && (mem->size >= size))
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
	t_alloc		*tmp;

	tmp = NULL;
	if (!type && size <= TINY_SIZE && g_mem->tiny->last)
		tmp = (t_alloc*)eco_search(size, type);
	else if (type && size <= SMALL_SIZE && g_mem->small->last)
		tmp = (t_alloc*)eco_search(size, type);
	if (tmp)
		((t_zone*)tmp->master)->size -= tmp->size + sizeof(t_alloc);
	return ((void*)tmp);
}
