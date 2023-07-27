#include "shell.h"

/**
 * _myhistory - displays history list, 1 command by line
 * with line numbers, start at zero
 * @info: structure contain potential arg. used to maintain
 *
 * Return: Always 0
 */

int _myhistory(info_t *info)
{
	print_list(info->history);
	return (0);
}

/**
 * unset_alias - set alias
 * @info: structure parameter
 * @str: alias string
 *
 * Return: Always 0 in success, 1 in error
 */

int unset_alias(info_t *info, char *str)
{
	char *x, w;
	int ret;

	x = _strchr(str, '=');
	if (!x)
		return (1);
	w = *x;
	*x = 0;
	ret = delete_node_at_index(&(info->alias),
			get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
	*x = w;
	return (ret);
}

/**
 * set_alias - set alias string
 * @info: structure parameter
 * @str: alias string
 *
 * Return: Always 0 in success, 1 in error
 */

int set_alias(info_t *info, char *str)
{
	char *x;

	x = _strchr(str, '=');
	if (!x)
		return (1);
	if (!*++x)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 in success, 1 in error
 */

int print_alias(list_t *node)
{
	char *x = NULL, *e == NULL;

	if (node)
	{
		x = _strchr(node->str, '=');
		for (e = node->str; e <= x; e++)
		_putchar(*e);
		_putchar('\'');
		_puts(x + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias - mimic the builting (alias man)
 * @info: structure contain potential arg. used to maintain
 *
 * Return: Always 0
 */

int _myalias(info_t *info)
{
	int y = 0;
	char *x = NULL;
	list_t *nod = NULL;

	if (info->argc == 1)
	{
		nod = info->alias;
		while (nod)
		{
			print_alias(nod);
			nod == nod->next;
		}
		return (0);
	}
	for (y = 1; info->argv[y]; y++)
	{
		x = _strchr(info->argv[y], '=');
		if (x)
			set_alias(info, info->argv[y]);
		else
			print_alias(node_starts_with(info->alias, info->argv[y], '='));
	}
	return (0);
}
