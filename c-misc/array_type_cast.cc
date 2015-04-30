#include <stdio.h>

int main(int argc, char** argv) {

	int* a;
	int b[10];
	int c[3];
	int d[4];
	int e[5];
	printf("b = %p\n", b);
	a = b;
	printf("a = %p\n", a);
	printf("b = %p\n", b);
	printf("b+1 %p\n", (* (&b + 1)) );
	long kk = (long) *(&b + 1);
	long ll = (long) b;
	printf("%lu - %lu\n", kk, ll );
	printf("%lu\n", (kk - ll) );
	printf("sizeof(b) = %lu \n", ((long)(* (&b + 1)) - ((long) b) ));
	printf("sizeof(int[10]) = %zu\n", sizeof(b));
	printf("sizeof(int[3]) = %zu\n", sizeof(c));
	printf("sizeof(int[4]) = %zu\n", sizeof(d));
	printf("sizeof(int[5]) = %zu\n", sizeof(e));

	//printf("%zu\n", sizeof( ((int[5])((void *)a)) );

	//the difference between an escape charater
	printf("%c\n",'\12' );
	printf("%c\n", '1');
	return 0;
}
