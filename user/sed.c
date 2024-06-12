#include <lib.h>
#define STR_LEN 64

int strncmp(const char *s1, const char *s2, u_int n) {
	char u1, u2;

	while (n-- > 0) {
		u1 = *s1++;
		u2 = *s2++;
		if (u1 != u2) {
			return u1 - u2;
		}
		if (u1 == '\0') {
			return 0;
		}
	}
	return 0;
}

void sed(int f, char *cmd) {
	if (*cmd == 's') {
		char old[STR_LEN], new[STR_LEN];

		// Parse arguments to get `old` and `new`.
		int cur = 1, i = 0, j = 0;
		while (cmd[++cur] != '/') {
			old[i++] = cmd[cur];
		}
		old[i] = '\0';
		while (cmd[++cur] != '/') {
			new[j++] = cmd[cur];
		}
		new[j] = '\0';

		// Your code here. (1/2)

	} else {
		int start = 0, end = 0;

		// Parse arguments to get `start` (`end`) line.
		while ('0' <= *cmd && *cmd <= '9') {
			start = start * 10 + (*cmd++ - '0');
		}
		end = start;
		if (*cmd == ',') {
			end = 0;
			cmd++;
			while ('0' <= *cmd && *cmd <= '9') {
				end = end * 10 + (*cmd++ - '0');
			}
		}

		// Your code here. (2/2)
	}
}

int main(int argc, char **argv) {
	int f;

	if (argc == 2) {
		sed(0, argv[1]);
	} else {
		f = open(argv[2], O_RDWR);
		if (f < 0) {
			user_panic("can't open %s", argv[2]);
		} else {
			sed(f, argv[1]);
			close(f);
		}
	}
	return 0;
}
