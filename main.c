// #include "malloc.h"
#include "malloc.h"
// #include <stdio.h>

// static int	is_power_of_two(int n)
// {
// 	return (n && (!(n & (n - 1))));
// }

// static int	find_position(int n)
// {
// 	int i;
// 	int pos;

// 	if (!is_power_of_two(n))
// 		return (-1);
// 	i = 1;
// 	pos = 1;
// 	while (!(i & n))
// 	{
// 		i = i << 1;
// 		++pos;
// 	}
// 	return (pos);
// }

// int			align_number(int input, int align)
// {
// 	int ret;
// 	int pos;

// 	pos = find_position(align) - 1;
// 	if (pos < 0)
// 	{
// 		printf("Critical error align is not ˆ2\n");
// 		return (-1); //ERROR Should never go there ... align is not ^2 (Si c'est comme ça faut switch sur la boucle de merde mdr, genre si pagesize renvoie un autre truc qu'une puissance de 2... devrait pas arriver)
// 	}
// 	if ((input % align))
// 		ret = 1;
// 	else
// 		ret = 0;
// 	ret = (((input >> pos) + ret) * align);
// 	if (ret == input)
// 		return (1);
// 	return (0);
// }

// int			main(int argc, char const **argv)
// {
// 	int j = 976 * 16;
// 	 for (int i = j; i < 17000; i += 1)
// 	 {
// 	 	if (align_number((i + 48) * 100, 4096))
// 	 		printf("truc %d\n", i);
// 		 // printf("Input = %d, Output = %d\n", i, );
// 	 }
// 	return (0);
// }

void hexDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        printf("  NEGATIVE LENGTH: %i\n",len);
        return;
    }

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

int		main()
{
	malloc(12);
	char *str = malloc(16);
	char *mdr;
	memset(str, 0, 4200);
	write(1, str, 42);
	write(1, "\n", 1);
	memset(str, 'F', 16);
	// write(1, str, 42);
	// write(1, "\n", 1);
	mdr =malloc(34);
	memset( mdr, '0', 34);
	write(1, str, 42);
	write(1, "\n", 1);
	hexDump( "yey", (void*)str - 48, 4096);
	return (0);
}