/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlistrat <vlistrat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 13:25:51 by vlistrat          #+#    #+#             */
/*   Updated: 2019/05/06 13:26:20 by vlistrat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

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
