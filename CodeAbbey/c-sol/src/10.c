#include <unistd.h> /*for getop */
#include <stdint.h>
#include <errno.h> /*for errno*/
#include <stdio.h> /*provided perror*/
#include <stdlib.h> /*provides exit*/
#include <assert.h>

unsigned joe(unsigned N, unsigned K) {
	unsigned n, g;
	g = 0;
	for(n = 2; n <= N; ++n) {
		g = (g+K)%n;
	}
	return g;
}
#define UPPER_LIMIT 10000

int main(int argc, char* argv[]) {
	unsigned N;
	int rc;
	rc = scanf("%u ", &N);
	if(rc != 1 || !rc) {
		errno = EINVAL;
		perror("Enter only one integer > 0 on the first line.");
		exit(-1);
	}
	signed* m_result = (signed*)calloc(N, sizeof(signed));
	if(m_result == NULL) {
		perror("Failed to allocate m_result buffer.");
		exit(-1);
	}
	signed* b_result = (signed*)calloc(N, sizeof(signed));
	if(b_result == NULL) {
		perror("Failed to allocate b_result buffer.");
		exit(-1);
	}
	unsigned ii;
	signed x1, x2, y1, y2;
	for(ii = 0; ii < N; ++ii) {
		scanf("%d %d %d %d ", &x1, &y1, &x2, &y2);
		m_result[ii] = (y2 - y1)/(x2 - x1);
		b_result[ii] = y1 - m_result[ii] * x1;
	}
	for(ii = 0; ii < N; ++ii) {
		printf("(%d %d) ", m_result[ii], b_result[ii]);
	}
	printf("\n");
	return 0;
}
