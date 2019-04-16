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
		if (!tmp)
		{
			// show_alloc_mem();
			ft_putstr("ret NULL\n");
		}
		if (tmp && tmp->free)
			ft_putstr("already freed\n");
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
			ft_memmove(ret, tmp->data, tmp->size);
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
	ft_putstr("IN R> ");
	ft_print_mem(ptr);
	ft_putchar('\n');
	if ((ret = re_alloc(ptr, size)) == NULL)
	{
		ft_putstr("OUT R> ");
		ft_print_mem(ptr);
		ft_putchar('\n');
		return (ptr);
	}
	ft_putstr("OUT R> ");
	ft_print_mem(ret);
	ft_putchar('\n');
	return (ret);
}