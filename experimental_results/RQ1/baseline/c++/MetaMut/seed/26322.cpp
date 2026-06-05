
int *a(void) { return 0; }
typedef void (*IntermediarySignature)(void);
template <IntermediarySignature b> void z(void) {}

int main(int argc, char **argv)
{
	z<(IntermediarySignature)a>();
	z<reinterpret_cast<IntermediarySignature>(a)>();
	return 0;
}

