#include "malloc.h"

int		main(void)
{
	void	*ptr1, *ptr2, *ptr3, *ptr4, *ptr5;

	ft_putnbr(TINY_ZONE_SIZE - sizeof(t_zone));
	ft_putchar('\n');
	ptr1 = malloc(50);
	ptr2 = malloc(100);
	ptr3 = malloc(200);
	ptr4 = malloc(300);
	ptr5 = malloc(500);
	free(ptr1);
	free(ptr2);
	free(ptr3);
	free(ptr4);
	free(ptr5);
}