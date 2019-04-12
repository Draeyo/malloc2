#include "malloc.h"

static void		print_hex(unsigned long nb)
{
	const char	*tmp = "0123456789abcdef";

	if (nb >= 16)
	{
		print_hex(nb / 16);
		print_hex(nb % 16);
	}
	else
		ft_putchar(tmp[nb]);
}

static void		print_memory(void *ptr)
{
	ft_putstr("0x");
	print_hex((unsigned long)ptr);
}

static int		print_mem(t_zone *zone, char *type)
{
	int			ret_size;
	t_alloc		*mem;

	ret_size = 0;
	mem = NULL;
	while (zone)
	{
		ft_putstr(type);
		print_memory((void*)zone);
		ft_putchar('\n');
		mem = zone ? (void*)zone + sizeof(t_zone) : NULL;
		while (mem && mem->next)
		{
			if (!mem->free)
			{
				print_memory((void*)mem->data);
				ft_putstr(" - ");
				print_memory((void*)mem->data + (long)mem->size);
				ft_putstr(" : ");
				ft_putnbr(mem->size);
				ret_size += mem->size;
				ft_putendl(" octets");
			}
			mem = mem->next;
		}
		zone = zone->next;
	}
	return (ret_size);
}

void	show_alloc_mem(void)
{
	int		total_size;

	total_size = 0;
	total_size += print_mem(g_mem->tiny, "TINY : ");
	total_size += print_mem(g_mem->small, "SMALL : ");
	total_size += print_mem(g_mem->large, "LARGE : ");
	if (total_size)
	{
		ft_putstr("Total : ");
		ft_putnbr(total_size);
		ft_putendl(" octets");
	}
}