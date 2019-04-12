#include "malloc.h"

void	*calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = NULL;
	if (!count || !size)
	{
		count = 1;
		size = 1;
	}
	// ft_putnbr(count * size);
	// ft_putchar('\n');
	if ((ptr = malloc(count * size)) == NULL)
		return (NULL);
	ft_memset(ptr, 0, count * size);
	// ft_print_mem(ptr);
	// ft_putchar('\n');
	return (ptr);
}
