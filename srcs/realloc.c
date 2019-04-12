#include "malloc.h"

static void		*resize_alloc(t_alloc *mem, size_t size)
{
	void	*ret;

	ret = NULL;
	if ((ret = malloc(size)) == NULL)
		return (NULL);
	ft_memmove(ret, mem->data, mem->size);
	mem->free = 1;
	return (ret);
}

static void		*re_alloc(void *ptr, size_t size)
{
	t_alloc		*tmp;
	void		*ret;

	tmp = NULL;
	ret = NULL;
	if (ptr == NULL)
		return (malloc(size));
	tmp = find_alloc(ptr);
	if (tmp == NULL || (tmp != NULL && tmp->free))
		return (NULL);
	if ((tmp != NULL && size == 0) || tmp->size > size)
	{
		ret = malloc(size);
		if (ret != NULL)
			tmp->free = 1;
		return (ret);
	}
	// join free
	return (resize_alloc(tmp, size));
}

void			*realloc(void *ptr, size_t size)
{
	void	*ret;

	ret = NULL;
	size = ft_mem_padding(size);
	if ((ret = re_alloc(ptr, size)) == NULL)
		return (ptr);
	return (ret);
}