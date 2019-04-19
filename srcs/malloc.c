#include "malloc.h"
#include <signal.h>
#include <execinfo.h>

t_overall *g_mem = NULL;

static void		init_glob()
{
	g_mem = mmap(0, PAGE_SIZE, PROT, FLAGS, -1, 0);
	bzero(g_mem, sizeof(t_overall));
}

void	print_trace(int	nb)
{
	// void *array[10];
	// size_t size;
	// char **strings;
	// size_t i;

	// g_mem->debug = 1;
	// size = backtrace(array, 10);
	// strings = backtrace_symbols(array, size);

	// (void)nb;
	// for (i = 0; i < size; i++)
	// 	ft_putendl(strings[i]);

	// ft_putstr("SIGNAL : ");
	// ft_putnbr(nb);
	// ft_putchar('\n');
	// free(strings);
	(void)nb;
	show_alloc_mem();
	exit(1);
}

void	*malloc(size_t size)
{
	void	*ret;

	// for (int i = 0; i < 18; ++i)
	// 	signal(i, &print_trace);
	ret = NULL;
	if (!g_mem)
		init_glob();
	size = align_number(size, 16);
	if (size <= 0 || size >= SIZE_MAX)
		return (NULL);
	else if (size <= TINY_SIZE)
		ret = alloc_tiny(size);
	else if (size <= SMALL_SIZE)
		ret = alloc_small(size);
	else
		ret = alloc_large(size);
	if (ret == MAP_FAILED)
		return (NULL);
	ft_putstr("+ ");
	ft_print_mem(ret);
	ft_putstr(" -> ");
	ft_print_mem(((t_alloc*)ret)->master);
	//ft_putstr(" -> ");
	//ft_print_mem(((t_zone*)((t_alloc*)ret)->master)->end);
	ft_putchar('\n');
	return (ret);
}
