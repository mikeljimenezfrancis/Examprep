#include <stdio.h>
#include <ctype.h>

static void unexpected(char c);
static int accept(char **s, char c);
static int expect(char **s, char c, int *err);
static int parse_factor(char **s, int *err);
static int parse_term(char **s, int *err);
static int parse_expr(char **s, int *err);

static void unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
}

static int accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return(1);
	}
	return(0);
}

static int expect(char **s, char c, int *err)
{
	if (accept(s, c))
		return(1);
	unexpected(**s);
	*err = 1;
	return(0);
}

static int parse_factor(char **s, int *err)
{
	int val;

	if (isdigit((unsigned char)**s))
	{
		val = **s - '0';
		(*s)++;
		return(val);
	}
	if (accept(s, '('))
	{
		val = parse_expr(s, err);
		if (*err)
			return(0);
		expect(s, ')', err);
		return(val);
	}
	unexpected(**s);
	*err = 1;
	return(0);
}

static int parse_term(char **s, int *err)
{
	int left;

	left = parse_factor(s, err);
	while (!*err && accept(s, '*'))
		left *= parse_factor(s, err);
	return(left);
}

static int parse_expr(char **s, int *err)
{
	int left;

	left = parse_term(s, err);
	while (!*err && accept(s, '+'))
		left += parse_term(s, err);
	return(left);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return(1);
	int err = 0;
	char *s = argv[1];
	int result = parse_expr(&s, &err);
	if (!err && *s)
	{
		unexpected(*s);
		err = 1;
	}
	if (err)
		return(1);
	printf("%d\n", result);
	return(0);
}