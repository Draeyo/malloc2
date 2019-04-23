#include "malloc.h"

// static void		*resize_alloc(t_alloc *mem, size_t size)
// {
// 	void	*ret;

// 	ret = NULL;
// 	if ((ret = malloc(size)) == NULL)
// 		return (NULL);
// 	ft_memmove(ret, mem->data, mem->size);
// 	mem->free = 1;
// 	return (ret);
// }

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
	{
		ft_putstr("ptr not found or already freed : ");
		ft_print_mem(ptr);
		ft_putchar('\n');
		return (NULL);
	}
	if (tmp->size < size)
	{
		ret = malloc(size);
		if (ret != NULL)
		{
			memmove(ret, tmp->data, tmp->size);
			tmp->free = 1;
		}
		return (ret);
	}
	// join free
	//return (resize_alloc(tmp, size));
	return (tmp->data);
}

void			*realloc(void *ptr, size_t size)
{
	void	*ret;

	ret = NULL;
	size = align_number(size, 16);
	if ((ret = re_alloc(ptr, size)) == NULL)
		return (ptr);
	return (ret);
}
