/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlistrat <vlistrat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 13:32:43 by vlistrat          #+#    #+#             */
/*   Updated: 2019/05/06 14:04:23 by vlistrat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int		print_each(t_alloc *mem, int ret_size)
{
	print_memory((void*)mem->data);
	ft_putstr(" - ");
	print_memory((void*)mem->data + (long)mem->size);
	ft_putstr(" : ");
	ft_putnbr(mem->size);
	ret_size += mem->size;
	ft_putendl(" octets");
	return (ret_size);
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
		ft_putstr(" : ");
		print_memory((void*)zone);
		mem = zone ? (void*)zone + sizeof(t_zone) : NULL;
		while (mem)
		{
			if (!mem->free)
				ret_size += print_each(mem, ret_size);
			mem = mem->next;
		}
		zone = zone->next;
	}
	return (ret_size);
}

void			show_alloc_mem(void)
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
