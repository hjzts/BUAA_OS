#include <lib.h>

// Option values of [-h height] and [-w width], respectively.
int h = 25;
int w = 80;

const int INT_MAX = 2147483647;
char input[8192];

// Overview:
//   Interpret integer in range (0, 2^31) in the string 'arg' when handling option 'argname'.
//
// Post-Condition:
//   Return interpreted integer on success.
//   Print error message and halt on error.
int atoi(char *argname, char *arg)
{
	int r = 0;

	while (*arg != '\0')
	{
		if (*arg < '0' || *arg > '9')
		{
			user_panic("%s: invalid argument", argname);
		}
		if (r > (INT_MAX - (*arg - '0')) / 10)
		{
			user_panic("%s: invalid argument", argname);
		}
		r = r * 10 + (*arg - '0');
		arg++;
	}

	if (r <= 0)
	{
		user_panic("%s: invalid argument", argname);
	}

	return r;
}

int main(int argc, char **argv)
{
	// Handle arguments.
	for (int i = 1; i < argc; i++)
	{
		// Handle option [-h height].
		if (strcmp(argv[i], "-h") == 0)
		{
			if (i + 1 >= argc)
			{
				user_panic("%s: argument not found", argv[i]);
			}
			h = atoi(argv[i], argv[i + 1]);
			i++;
			continue;
		}

		// Handle option [-w width].
		// You may take [-h height] as an example.
		/* Your code here. (1/6) */
		if (strcmp(argv[i], "-w") == 0)
		{
			if (i + 1 >= argc)
			{
				user_panic("%s: argument not found", argv[i]);
			}
			w = atoi(argv[i], argv[i + 1]);
			i++;
			continue;
		}
		int f;
		// Handle argument [file].
		// If 'stdin' is console, no file is opened yet.
		// In this case, open the file and duplicate its file descriptor to 'stdin'.
		if (iscons(0))
		{
			/* Your code here. (2/6) */
			f = open(argv[i], O_RDONLY);
			if (f < 0)
			{
				user_panic("can't open %s: %d", argv[i], f);
			}
			else
			{
				dup(f, 0);
				close(f);
			}
		}

		// Otherwise, there are more than one files in the arguments.
		// Print the corresponding error message and halt.
		/* Your code here. (3/6) */
		else
		{
			user_panic("too much arguments");
		}
	}

	// No file is opened. Print the corresponding error message and halt.
	if (iscons(0))
	{
		/* Your code here. (4/6) */
		user_panic("bad usage");
	}

	// Copy 'stdin' to 'stdout' when 'stdout' is not console.
	if (!iscons(1))
	{
		/* Your code here. (5/6) */
		dup(0, 1);
		return 0;
	}

	// Handle interactive commands. You may use 'syscall_cgetc()' to get the console input.
	// You can define variables and functions other than given ones for your convenience.
	/* Your code here. (6/6) */
	char ch;
	int i = 0;
	while ((ch = syscall_cgetc()) != '\n')
	{
		input[i] = ch;
		i++;
	}
	input[i] = '\0';
	return 0;
}
