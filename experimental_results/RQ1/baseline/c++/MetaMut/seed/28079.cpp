
template <class T> void
jm (T cv)
{
#pragma omp cancel parallel if (cv)
}

