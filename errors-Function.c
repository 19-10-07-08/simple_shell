#include "shell.h"

/**
 * _eputs - print an input string
 * @str: the string to  print
 *
 * Return: Nothing
 */
void _eputs(char *str)
{
	int x = 0;

	if (!str)
		return;
	while (str[x] != '\0')
	{
		_eputchar(str[x]);
		x++;
	}
}

/**
 * _eputchar - write the char c to stderr
 * @c: The char to print
 *
 * Return: On success 1
 * On error -1 is returned and errno is set appropriately
 */
int _eputchar(char c)
{
	static int x;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || x >= WRITE_BUF_SIZE)
	{
		write(2, buf, x);
		x = 0;
	}
	if (c != BUF_FLUSH)
		buf[x++] = c;
	return (1);
}

/**
 * _putfd - write the char c to given fd
 * @c: The char to print
 * @fd: The filedescriptor
 *
 * Return: On success 1
 * On error -1 is returned and errno is set appropriately
 */
int _putfd(char c, int fd)
{
	static int x;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || x >= WRITE_BUF_SIZE)
	{
		write(fd, buf, x);
		x = 0;
	}
	if (c != BUF_FLUSH)
		buf[x++] = c;
	return (1);
}

/**
 * _putsfd - print an input string
 * @str: the string to be printed
 * @fd: the filedescriptor
 *
 * Return: the number of char put
 */
int _putsfd(char *str, int fd)
{
	int x = 0;

	if (!str)
		return (0);
	while (*str)
	{
		x += _putfd(*str++, fd);
	}
	return (x);
}
