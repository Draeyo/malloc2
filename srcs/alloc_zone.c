#include "malloc.h"

t_zone	*alloc_zone(size_t size)
{
	t_zone		*new;

	new = NULL;
	size = align_number(size, 4096);
	if ((new = mmap(0, size, PROT, FLAGS, -1, 0)) == MAP_FAILED)
		return (NULL);
	ft_bzero(new, sizeof(t_zone) + sizeof(t_alloc));
	new->start = (void*)new + sizeof(t_zone);
	ft_bzero(new->start, sizeof(t_alloc));
	// ft_putstr("START : ");
	// ft_print_mem(new->start);
	// ft_putchar('\n');
	new->end = (void*)new + size;
	new->last = NULL;
	new->next_safe = (void*)new->start;
	new->next = NULL;
	new->size = size - sizeof(t_zone);
	return (new);
}