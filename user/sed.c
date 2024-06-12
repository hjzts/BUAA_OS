#include <lib.h>
#define STR_LEN 64
char out[8192];
char buf[8192];
char lines[1024];

int strncmp(const char *s1, const char *s2, u_int n)
{
	char u1, u2;

	while (n-- > 0)
	{
		u1 = *s1++;
		u2 = *s2++;
		if (u1 != u2)
		{
			return u1 - u2;
		}
		if (u1 == '\0')
		{
			return 0;
		}
	}
	return 0;
}

int readline(int line_num)
{
	// cong line_num kaishi du yi hang
	int n = 0;
	for (int i = line_num, j = 0;; i++, j++)
	{
		lines[j] = buf[i];
		n++;
		if (buf[i] == '\0')
			break;
		if (buf[i] == '\n' || buf[i] == '\r')
			break;
	}
	// printf("n %d\n",n);
	return n;
}

void sed(int f, char *cmd)
{
	if (*cmd == 's')
	{
		char old[STR_LEN], new[STR_LEN];

		// Parse arguments to get `old` and `new`.
		int cur = 1, i = 0, j = 0;
		int len1 = 0, len2 = 0;
		while (cmd[++cur] != '/')
		{
			old[i++] = cmd[cur];
			len1++;
		}
		old[i] = '\0';
		while (cmd[++cur] != '/')
		{
			new[j++] = cmd[cur];
			len2++;
		}
		new[j] = '\0';

		// Your code here. (1/2)
		int line = 0;
		i = 0;
		j = 0;
		int n = read(f, buf, (long)sizeof buf);
		// printf("%d %d\n", len1, len2);
		for (i = 0, j = 0; i < n; i++, j++)
		{
			if (strncmp(&buf[i], old, len1) == 0)
			{
				// printf("%d \n", i);
				for (int k = 0; k < len2; k++)
				{
					out[j + k] = new[k];
					// printf("%c ", new[k]);
				}
				i += len1-1;
				j += len2-1;
				continue;
			}
			out[j] = buf[i];
			// printf("%d %d\n", i, j);
		}
		seek(f, 0);
		ftruncate(f, 0);
		if (f != 0)
			write(f, out, j);
		else
		{
			write(1, out, j);
		}
	}
	else
	{
		int start = 0, end = 0;

		// Parse arguments to get `start` (`end`) line.
		while ('0' <= *cmd && *cmd <= '9')
		{
			start = start * 10 + (*cmd++ - '0');
		}
		end = start;
		if (*cmd == ',')
		{
			end = 0;
			cmd++;
			while ('0' <= *cmd && *cmd <= '9')
			{
				end = end * 10 + (*cmd++ - '0');
			}
		}

		// Your code here. (2/2)
		int line = 1;
		int n = read(f, buf, (long)sizeof buf);
		// for (int i = 0; i < n ;i++) {
		// 	printf("%c", buf[i]);
		// }
		int r;
		for (int i = 0; i < n; i++)
		{
			int len = readline(i);
			// for (int j = 0; j < len; j++)
			// {
			// 	printf("%c ", lines[j]);
			// }
			if (line >= start && line <= end)
			{
				// printf("%d %d %d\n", line, start, end);
				if ((r = write(1, lines, len)) != len)
				{
					user_panic("write error copying %s: %d", lines, r);
				}
			}
			i += len - 1;
			line++;
			if (line > end)
				break;
		}
	}
}

int main(int argc, char **argv)
{
	int f;

	if (argc == 2)
	{
		sed(0, argv[1]);
	}
	else
	{
		f = open(argv[2], O_RDWR);
		if (f < 0)
		{
			user_panic("can't open %s", argv[2]);
		}
		else
		{
			sed(f, argv[1]);
			close(f);
		}
	}
	return 0;
}
