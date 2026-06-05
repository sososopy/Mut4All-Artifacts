
void* ArgMappers; // passed in as a function argument.

    if (ArgMappers) {
      fprintf(stderr, "ArgMappers is not nullptr %p, I = %d\n", ArgMappers, I);
      if (ArgMappers[I]) {

