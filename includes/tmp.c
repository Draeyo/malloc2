#include "malloc.h"

static void		*find_large(void *ptr)
{
	t_zone 		*tmp;
	void		*ret;

	tmp = zone;
	ret = NULL;
	while (tmp)
	{
		if (((void)tmp + sizeof(t_zone)) == ptr)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

static void		*find_by_type(t_zone *zone, void *ptr)
{
	t_zone 		*tmp;
	t_alloc		*mem;

	tmp = zone;
	mem = NULL;
	while (tmp)
	{
		mem = tmp->first ? tmp->first : NULL;
		while (mem)
		{
			if ((void*)mem->data == ptr)
				return (mem);
			mem = mem->next;
		}
		tmp = tmp->next;
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