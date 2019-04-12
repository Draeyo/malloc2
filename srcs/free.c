#include "malloc.h"

static void		free_large(void *mem)
{
	t_zone	*tmp;
	t_alloc	*tmp_mem;

	tmp = g_mem->large ? g_mem->large : NULL;
	tmp_mem = NULL;
	if (!tmp)
		return ;
	while (tmp && tmp->next)
	{
		tmp_mem = (void*)tmp->next + sizeof(t_zone);
		if (mem == (void*)tmp_mem->data)
		{
			if (tmp == g_mem->large_last && tmp != g_mem->large)
			{
				// A VERIFIER
				g_mem->large_last = tmp;
				g_mem->large_last->last = (void*)tmp + sizeof(t_zone);
			}
			tmp->next = tmp->next->next;
			munmap(tmp->next, tmp_mem->size);
			return ;
		}
		tmp = tmp->next;
	}
}	

// void		switch_last(t_alloc *mem)
// {
// 	t_alloc		*tmp;

// 	tmp = NULL;
// 	if (mem == g_mem->tiny_last->last)
// 	{
// 		tmp = g_mem->tiny_last->last;
// 		g_mem->tiny_last->last = tmp->prev;
// 	}
// 	else if (mem == g_mem->small_last->last)
// 	{
// 		tmp = g_mem->small_last->last;
// 		g_mem->small_last->last = tmp->prev;
// 	}
// }

void		free(void *ptr)
{
	t_alloc		*tmp;
	t_zone		*zone;

	tmp = NULL;
	zone = NULL;
	if (!ptr)
		return ;
	tmp = find_alloc(ptr);
	if (tmp && tmp->size > SMALL_SIZE)
	{
		free_large((void*)tmp - sizeof(t_alloc) - sizeof(t_zone));
		return ;
	}
	if (tmp)
	{
		((t_zone*)tmp->master)->size += tmp->size + sizeof(t_alloc);
		tmp->free = 1;
	}
}