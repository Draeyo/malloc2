static int	is_power_of_two(int n)
{
	return (n && (!(n & (n - 1))));
}

static int	find_position(int n)
{
	int i;
	int pos;

	if (!is_power_of_two(n))
		return (-1);
	i = 1;
	pos = 1;
	while (!(i & n))
	{
		i = i << 1;
		++pos;
	}
	return (pos);
}

int			align_number(int input, int align)
{
	int ret;
	int pos;

	pos = find_position(align) - 1;
	if (pos < 0)
		return (-1);
	if ((input % align))
		ret = 1;
	else
		ret = 0;
	ret = (((input >> pos) + ret) * align);
	return (ret);
}