#include <stdio.h>
int main()
{
    int n;
    scanf("%d", &n);

    int flag = 1;
    int s[10];
    int len = 0;
    for (len = 0; n; len++) {
        s[len] = n % 10;
        n /= 10;
    }
    int i = 0, j = len - 1;
    while (i < j) {
        if (s[i] == s[j]) {
            i++;
            j--;
        } else {
            flag = 0;
            break;
        }
    }

    if (flag) {
        printf("Y\n");
    } else {
        printf("N\n");
    }
    return 0;
}
