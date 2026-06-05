
#include <stdio.h>
#include <pthread.h>

void g(bool x) {
	printf("%d\n", x);
}

void* f(void* arg) {
	static __thread bool i = true;
	g(i);
	return NULL;
}

int main() {
	pthread_t a, b;
	pthread_create(&a, NULL, &f, NULL);
	pthread_create(&b, NULL, &f, NULL);
	pthread_join(a, NULL);
	pthread_join(b, NULL);
	return 0;
}
