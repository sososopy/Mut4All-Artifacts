
#include <stdio.h>
#include <stdlib.h>

struct Expression {
	Expression *next;
	Expression *right;
	void *operator new(size_t);
	void operator delete(void *);
};

void * Expression::operator new(size_t sz)
{
	return malloc(sz);
}

void Expression::operator delete(void *p)
{
	Expression *e = (Expression*)p;
	printf("%p : %p\n", e, e->next);
	while(e->next)
	{
		Expression *f = e->next;
		//pretend doing something
		printf("delete is doing something with %p\n", e->next);
		e = f;
	}
	free(p);
}

int main()
{
	Expression *etmp, *ebase;
	etmp = ebase = new Expression();
	for(int i = 0; i < 10; ++i)
	{
		etmp->next = new Expression();
		etmp->right = new Expression();
		etmp->right->next = NULL;
		etmp->next->right = new Expression();
		etmp->next->right->next = NULL;
		etmp->next->right->right = NULL;
		etmp = etmp->next;
	}
	etmp->next = NULL;

	Expression *e, *e0, *e1, *efirst, **ep;

	if ((efirst = ebase)) {
		ep = &e0;
		while((e = efirst)) {
			*ep = e1 = e->right;
			efirst = e->next;
			e->next = 0; //!! with -O2 this line is eliminated
			delete e;
			while((e = e1->next))
				e1 = e;
			ep = &e1->next;
		}
	}

	return 0;
}
