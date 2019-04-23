#include "malloc.h"
#include <signal.h>
#include <execinfo.h>

t_overall *g_mem = NULL;

static void		init_glob()
{
	g_mem = mmap(0, PAGE_SIZE, PROT, FLAGS, -1, 0);
	bzero(g_mem, sizeof(t_overall));
}

void	*malloc(size_t size)
{
	void	*ret;

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
	return (ret);
}
