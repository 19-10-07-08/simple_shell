#include "shell.h"

/**
 * input_buf - buffer command chained
 * @info: structer parameter
 * @buf: buffer adress
 * @len: len afress
 *
 * Return: bytes read
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t m = 0;
	size_t lent = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		m = getline(buf, &lent, stdin);
#else
		m = _getline(info, buf, &lent);
#endif
		if (m > 0)
		{
			if ((*buf)[m - 1] == '\n')
			{
				(*buf)[m - 1] = '\0'; /* remove trailing newline */
				m--;
			}
			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = m;
				info->cmd_buf = buf;
			}
		}
	}
	return (m);
}

/**
 * get_input - gets line minus the newline
 * @info: structur prameter
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t x, y, lent;
	ssize_t m = 0;
	char **buf_p = &(info->arg), *w;

	_putchar(BUF_FLUSH);
	m = input_buf(info, &buf, &lent);
	if (m == -1) /* EOF */
		return (-1);
	if (lent) /* we have commands left in the chain buffer */
	{
		y = x; /* init new iterator to current buf position */
		w = buf + x; /* get pointer for return */

		check_chain(info, buf, &y, x, lent);
		while (y < lent) /* iterate to semicolon or end */
		{
			if (is_chain(info, buf, &y))
				break;
			y++;
		}

		x = y + 1; /* increment past nulled ';'' */
		if (x >= lent) /* reached end of buffer? */
		{
			x = lent = 0; /* reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = m; /* pass back pointer to current command position */
		return (_strlen(m)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _getline() */
	return (w); /* return length of buffer from _getline() */
}

/**
 * read_buf - reads buffer
 * @info: strucure parameter
 * @buf: buffer string
 * @i: size string
 *
 * Return: r
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->readfd, buf, READ_BUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - gets next line input from STDIN
 * @info: parameter structure
 * @ptr: address pointer to buffer, preallocated or NULL
 * @length: size preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t x, lent;
	size_t e;
	ssize_t c = 0, n = 0;
	char *m = NULL, *new_p = NULL, *i;

	m = *ptr;
	if (m && length)
		n = *length;
	if (x == lent)
		x = lent = 0;

	c = read_buf(info, buf, &lent);
	if (c == -1 || (c == 0 && lent == 0))
		return (-1);

	i = _strchr(buf + x, '\n');
	e = i ? 1 + (unsigned int)(i - buf) : lent;
	new_p = _realloc(m, n, n ? n + e : e + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (m ? free(m), -1 : -1);

	if (n)
		_strncat(new_p, buf + x, e - x);
	else
		_strncpy(new_p, buf + x, e - x + 1);

	n += e - x;
	x = e;
	m = new_p;

	if (length)
		*length = s;
	*ptr = m;
	return (n);
}

/**
 * sigintHandler - block ctrl-C
 * @sig_num: signal numbers
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
