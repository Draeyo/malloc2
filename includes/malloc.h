/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlistrat <vlistrat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 13:57:30 by vlistrat          #+#    #+#             */
/*   Updated: 2019/05/06 14:02:22 by vlistrat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <pthread.h>
# include "libft.h"
# include "ft_printf.h"

# define PROT	PROT_READ | PROT_WRITE
# define FLAGS	MAP_ANON | MAP_SHARED

# define PAGE_SIZE			(size_t)getpagesize()
# define INFO_SIZE			(size_t)sizeof(t_alloc)

# define TINY_SIZE			(size_t) 976
# define SMALL_SIZE			(size_t) 16336

# define TINY_ZONE_SIZE		(size_t)(((TINY_SIZE + INFO_SIZE) * 100) + sizeof(t_zone))
# define SMALL_ZONE_SIZE	(size_t)(((SMALL_SIZE + INFO_SIZE) * 100) + sizeof(t_zone))

# define TINY_TYPE		1
# define SMALL_TYPE		2
# define LARGE_TYPE		3

typedef struct			s_alloc
{
	struct s_alloc		*next;
	struct s_alloc		*prev;
	size_t				size;
	size_t				free;
	void				*master;
	void				*data;
}						t_alloc;

typedef struct			s_zone
{
	struct s_zone		*next;
	void				*start;
	void				*end;
	void				*last;
	void				*next_safe;
	size_t				size;
}						t_zone;

typedef struct			s_overall
{
	t_zone				*tiny;
	t_zone				*tiny_last;
	t_zone				*small;
	t_zone				*small_last;
	t_zone				*large;
	t_zone				*large_last;
	int					debug;
}						t_overall;

extern t_overall		*g_mem;

void					free(void *ptr);
void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);
void					show_alloc_mem(void);

void					*eco_search(size_t size, size_t type);
void					*eco_alloc(size_t size, size_t type);

size_t					ft_mem_padding(size_t size);

t_zone					*alloc_zone(size_t size);

void					*alloc_tiny(size_t size);
void					*alloc_small(size_t size);
void					*alloc_large(size_t size);

void					*calloc(size_t count, size_t size);

void					*find_alloc(void *ptr);
void					*find_large(void *ptr);
void					*find_last_in_zone(void *zone);

size_t					align_number(size_t input, size_t align);

void					free_large(void *mem);

#endif
